// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleMPGameInstance.h"
#include "Engine/Engine.h"

UPuzzleMPGameInstance::UPuzzleMPGameInstance(const FObjectInitializer &ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));
}

void UPuzzleMPGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
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
}
