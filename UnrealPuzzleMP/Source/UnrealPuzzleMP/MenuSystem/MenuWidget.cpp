// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::Setup()
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

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
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

void UMenuWidget::SetMainMenuInterface(IMainMenuInterface* MainMenuInterface)
{
	this->MainMenuInterface = MainMenuInterface;
}