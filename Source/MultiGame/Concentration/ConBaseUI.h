// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "TextBlock.h"
#include "ConBaseUI.generated.h"

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

    UFUNCTION()
        void SimpleButtonClicked();
};
