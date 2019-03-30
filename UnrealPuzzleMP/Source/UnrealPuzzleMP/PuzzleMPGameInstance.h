// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MainMenuInterface.h"
#include "OnlineSubsystem.h"
#include "PuzzleMPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPUZZLEMP_API UPuzzleMPGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()
	
public:
	UPuzzleMPGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;
	virtual void LoadMainMenu() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(exec)
	void Host() override;

	UFUNCTION(exec)
	void Join(const FString & Address) override;

private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void CreateSession();

	IOnlineSessionPtr SessionInterface;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
};
