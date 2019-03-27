// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MainMenuInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success || !HostButton || !JoinButton || !CancelJoinButton)
		return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	CancelJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::SetMainMenuInterface(IMainMenuInterface* MainMenuInterface)
{
	this->MainMenuInterface = MainMenuInterface;
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	auto World = GetWorld();

	if (!World)
		return;

	auto PlayerController = World->GetFirstPlayerController();

	if (!PlayerController)
		return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	RemoveFromViewport();
	auto World = GetWorld();

	if (!World)
		return;

	auto PlayerController = World->GetFirstPlayerController();

	if (!PlayerController)
		return;

	FInputModeGameOnly inputMode;
	PlayerController->SetInputMode(inputMode);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::HostServer()
{
	if (!MainMenuInterface)
		return;

	MainMenuInterface->Host();
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher|| !JoinMenu)
		return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!MenuSwitcher || !MainMenu)
		return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}
