// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MainMenuInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRow.h"
#include "Components/TextBlock.h"

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

void UMainMenu::SetServerList(TArray<FServerData> Servers)
{
	auto World = this->GetWorld();

	if (!World)
		return;

	ServerList->ClearChildren();
	uint32 Index = 0;

	for (auto &Server : Servers)
	{
		auto ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);

		if (!ServerRow)
			return;

		ServerRow->ServerName->SetText(FText::FromString(Server.Name));
		ServerRow->Host->SetText(FText::FromString(Server.Host));
		ServerRow->Players->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Server.CurrentPlayers, Server.MaxPlayers)));
		ServerRow->Ping->SetText(FText::FromString(FString::Printf(TEXT("%d"), Server.Ping)));
		ServerRow->Setup(this, Index);
		++Index;

		ServerList->AddChild(ServerRow);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::HostServer()
{
	if (!MainMenuInterface)
		return;

	MainMenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MainMenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d."), SelectedIndex.GetValue());
		MainMenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}
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

	if (MainMenuInterface)
		MainMenuInterface->RefreshServerList();
}

void UMainMenu::OpenMainMenu()
{
	if (!MenuSwitcher || !MainMenu)
		return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::UpdateChildren()
{
	for (auto i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));

		if (Row)
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
	}
}
