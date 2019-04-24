// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleMPGameInstance.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("GameSession");
const static FName SERVER_NAME_KEY = TEXT("ServerName");
const static int32 MAX_SESSIONS = 100;

UPuzzleMPGameInstance::UPuzzleMPGameInstance(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));

	if (!MenuBPClass.Class || !InGameMenuBPClass.Class)
		return;

	MenuClass = MenuBPClass.Class;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzleMPGameInstance::Init()
{
	auto OSS = IOnlineSubsystem::Get();

	if (!OSS)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Found subsystem: %s"), *OSS->GetSubsystemName().ToString());
	SessionInterface = OSS->GetSessionInterface();

	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleMPGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzleMPGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzleMPGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzleMPGameInstance::OnJoinSessionComplete);
	}
}

void UPuzzleMPGameInstance::LoadMainMenu()
{
	auto PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
		return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzleMPGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionSearch->MaxSearchResults = MAX_SESSIONS;

		UE_LOG(LogTemp, Warning, TEXT("Begin session find"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzleMPGameInstance::LoadMenu()
{
	if (!MenuClass)
		return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!Menu)
		return;

	Menu->Setup();

	Menu->SetMainMenuInterface(this);
}

void UPuzzleMPGameInstance::LoadInGameMenu()
{
	if (!InGameMenuClass)
		return;

	auto InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

	if (!InGameMenu)
		return;

	InGameMenu->Setup();

	InGameMenu->SetMainMenuInterface(this);
}

void UPuzzleMPGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
		
	}
}

void UPuzzleMPGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
		return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzleMPGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting"));

	auto World = GetWorld();

	if (!World)
		return;

	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
}

void UPuzzleMPGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPuzzleMPGameInstance::OnFindSessionsComplete(bool Success)
{
	if (SessionSearch.IsValid() && Success && Menu)
	{
		UE_LOG(LogTemp, Warning, TEXT("End session find"));

		TArray<FServerData> ServerNames;

		for (auto &Result : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session: %s, Ping: %i"), *Result.GetSessionIdStr(), Result.PingInMs);

			FServerData ServerData;
			ServerData.Ping = Result.PingInMs;
			ServerData.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = ServerData.MaxPlayers - Result.Session.NumOpenPublicConnections;
			ServerData.Host = Result.Session.OwningUserName;

			FString ServerName;
			if (Result.Session.SessionSettings.Get(SERVER_NAME_KEY, ServerName))
				ServerData.Name = ServerName;
			else
				ServerData.Name = "Unknown";

			ServerNames.Add(ServerData);
		}

		Menu->SetServerList(ServerNames);
	}
}

void UPuzzleMPGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
			SessionSettings.bIsLANMatch = true;
		else
			SessionSettings.bIsLANMatch = false;

		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzleMPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
		return;

	FString Address;

	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could get connect string"));
		return;
	}

	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining: %s"), *Address));

	auto PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
		return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
