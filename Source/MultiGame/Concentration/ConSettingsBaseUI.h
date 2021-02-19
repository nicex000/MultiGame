// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Button.h"
#include "ComboBoxString.h"
#include "CoreMinimal.h"
#include "Slider.h"
#include "Blueprint/UserWidget.h"
#include "ConSettingsBaseUI.generated.h"

/**
 * 
 */
UCLASS()
class MULTIGAME_API UConSettingsBaseUI : public UUserWidget
{
	GENERATED_BODY()

		void NativeConstruct() override;
public:

	UPROPERTY(meta = (BindWidget))
		UButton* ApplyBtn;
	UPROPERTY(meta = (BindWidget))
		UButton* CancelBtn;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
		USlider* RowSizeSlider;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
		USlider* TotalPairsSlider;
	UPROPERTY(meta = (BindWidget))
		UComboBoxString* CardTypePicker;

	UFUNCTION()
		void ApplyClicked();

};
