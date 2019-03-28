// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleMPGameInstance.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


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
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
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
	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting"));

	auto World = GetWorld();

	if (!World)
		return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
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
