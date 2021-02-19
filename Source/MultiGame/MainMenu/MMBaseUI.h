// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Button.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Concentration/ConSettingsBaseUI.h"

#include "MMBaseUI.generated.h"

/**
 * 
 */
UCLASS()
class MULTIGAME_API UMMBaseUI : public UUserWidget
{
	GENERATED_BODY()

	void NativeConstruct() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UConSettingsBaseUI> ConSettingsUIClass;

	UConSettingsBaseUI* conSettingsUI;
	
	UPROPERTY(meta = (BindWidget))
		UButton* QuitBtn;
	UPROPERTY(meta = (BindWidget))
		UButton* ConStart;
	UPROPERTY(meta = (BindWidget))
		UButton* ConSettings;
	UPROPERTY(meta = (BindWidget))
		UButton* RailHeroStart;
	UPROPERTY(meta = (BindWidget))
		UButton* RailHeroSettings;


	UFUNCTION()
		void QuitButtonClicked();

	UFUNCTION()
		void ConcentrationStartClicked();

	UFUNCTION()
		void ConcentrationSettingsClicked();

	UFUNCTION()
		void ConcentrationSettingsExited();

	UFUNCTION()
		void RailHeroStartClicked();

	UFUNCTION()
		void RailHeroSettingsClicked();

	UFUNCTION()
		void RailHeroSettingsExited();
};
