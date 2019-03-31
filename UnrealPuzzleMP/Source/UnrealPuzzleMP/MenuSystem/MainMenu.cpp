// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MainMenuInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));

	if (!ServerRowBPClass.Class)
		return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success || !HostButton || !JoinButton || !CancelJoinButton || !JoinIpButton || !ExitButton)
		return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitPressed);
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
	/*if (!MainMenuInterface || !IpAddress)
		return;

	MainMenuInterface->Join(IpAddress->GetText().ToString());*/

	auto World = this->GetWorld();

	if (!World)
		return;

	auto ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);

	if (!ServerRow)
		return;

	ServerList->AddChild(ServerRow);
}

void UMainMenu::ExitPressed()
{
	auto World = GetWorld();

	if (!World)
		return;

	auto PlayerController = World->GetFirstPlayerController();

	if (!PlayerController)
		return;

	PlayerController->ConsoleCommand("quit");
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
