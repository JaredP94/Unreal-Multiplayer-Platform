// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPUZZLEMP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMainMenuInterface(IMainMenuInterface* MainMenuInterface);

	void Setup();

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void HostServer();

	UPROPERTY(meta = (BindWidget))
	class UButton* host;

	UPROPERTY(meta = (BindWidget))
	class UButton* join;

	IMainMenuInterface* MainMenuInterface;
};
