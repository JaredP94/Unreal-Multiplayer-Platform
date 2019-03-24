// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleMPGameInstance.h"

#include "PlatformTrigger.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


UPuzzleMPGameInstance::UPuzzleMPGameInstance(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (!MenuBPClass.Class)
		return;

	MenuClass = MenuBPClass.Class;
}

void UPuzzleMPGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
}

void UPuzzleMPGameInstance::LoadMenu()
{
	if (!MenuClass)
		return;

	auto Menu = CreateWidget<UUserWidget>(this, MenuClass);

	if (!Menu)
		return;

	Menu->AddToViewport();

	auto PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
		return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
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
