// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MainMenuInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success || !HostButton || !JoinButton || !CancelJoinButton || !JoinIpButton)
		return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	CancelJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	JoinIpButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::HostServer()
{
	if (!MainMenuInterface)
		return;

	MainMenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (!MainMenuInterface || !IpAddress)
		return;

	MainMenuInterface->Join(IpAddress->GetText().ToString());
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
