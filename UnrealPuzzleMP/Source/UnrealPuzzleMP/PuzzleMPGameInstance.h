// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MainMenuInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
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
	virtual void RefreshServerList() override;
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(exec)
	void Host(FString ServerName) override;

	UFUNCTION(exec)
	void Join(uint32 Index) override;

private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void CreateSession();
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	class UMainMenu* Menu;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	FString DesiredServerName;
};
