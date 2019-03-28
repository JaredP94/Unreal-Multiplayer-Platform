// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success || !BackButton || !QuitButton)
		return false;

	BackButton->OnClicked.AddDynamic(this, &UInGameMenu::PressedBack);
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::PressedQuit);

	return true;
}

void UInGameMenu::PressedBack()
{
	OnLevelRemovedFromWorld(GetWorld()->GetCurrentLevel(), GetWorld());
}

void UInGameMenu::PressedQuit()
{
	if (!MainMenuInterface)
		return;

	MainMenuInterface->LoadMainMenu();
}
