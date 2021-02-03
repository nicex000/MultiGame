// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "TextBlock.h"
#include "CanvasPanel.h"
#include "ConBaseUI.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class MULTIGAME_API UConBaseUI : public UUserWidget
{
	GENERATED_BODY()

	void NativeConstruct() override;
public:

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Attempts;
	
	UPROPERTY(meta = (BindWidget))
    UTextBlock* TotalPairs;
	
	UPROPERTY(meta = (BindWidget))
    UTextBlock* SuccessfulPairs;

	UPROPERTY(meta = (BindWidget))
	UButton* MatchQuit;


	// end screen stuff

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* EndPanel;
	
	UPROPERTY(meta = (BindWidget))
	UButton* EndRestart;

	UPROPERTY(meta = (BindWidget))
	UButton* EndSettings;

	UPROPERTY(meta = (BindWidget))
	UButton* EndQuit;

	UFUNCTION()
	void QuitButtonClicked();
	
	UFUNCTION()
	void SettingsButtonClicked();
	
	UFUNCTION()
	void RestartButtonClicked();
	
};
