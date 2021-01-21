// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ConGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIGAME_API AConGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AConGameMode();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
protected:
    /** Called when the game starts. */
    void BeginPlay() override;

    /** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
    TSubclassOf<UUserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
    UUserWidget* CurrentWidget;
};
