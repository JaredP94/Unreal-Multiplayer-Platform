// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"

#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(UMainMenu * InMainMenu, uint32 InIndex)
{
	MainMenuParent = InMainMenu;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	MainMenuParent->SelectIndex(Index);
}
