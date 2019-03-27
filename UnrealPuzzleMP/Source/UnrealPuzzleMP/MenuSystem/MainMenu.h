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

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinIpButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IpAddress;

	IMainMenuInterface* MainMenuInterface;
};
