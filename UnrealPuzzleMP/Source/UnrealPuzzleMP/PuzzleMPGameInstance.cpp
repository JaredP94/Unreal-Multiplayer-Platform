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
#include "OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"


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
}

void UPuzzleMPGameInstance::LoadMainMenu()
{
	auto PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
		return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzleMPGameInstance::LoadMenu()
{
	if (!MenuClass)
		return;

	auto Menu = CreateWidget<UMainMenu>(this, MenuClass);

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

void UPuzzleMPGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleMPGameInstance::OnCreateSessionComplete);

		FOnlineSessionSettings Settings;
		SessionInterface->CreateSession(0, TEXT("GameSession"), Settings);
	}
}

void UPuzzleMPGameInstance::Join(const FString & Address)
{
	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining: %s"), *Address));

	auto PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
		return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}
