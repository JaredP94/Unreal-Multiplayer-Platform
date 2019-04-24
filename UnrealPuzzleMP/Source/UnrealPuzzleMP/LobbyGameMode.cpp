// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "PuzzleMPGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NumberOfPlayers++;

	if (NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 5.f);
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	NumberOfPlayers--;
}

void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UPuzzleMPGameInstance>(GetGameInstance());

	if (!GameInstance)
		return;

	GameInstance->StartSession();

	bUseSeamlessTravel = true;

	auto World = GetWorld();

	if (!World)
		return;

	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}
