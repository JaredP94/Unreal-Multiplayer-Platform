// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MainMenuInterface.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
		return false;

	if (!host)
		return false;

	host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

void UMainMenu::SetMainMenuInterface(IMainMenuInterface* MainMenuInterface)
{
	this->MainMenuInterface = MainMenuInterface;
}

void UMainMenu::HostServer()
{
	if (!MainMenuInterface)
		return;

	MainMenuInterface->Host();
}
