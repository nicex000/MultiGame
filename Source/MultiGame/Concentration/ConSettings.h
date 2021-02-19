// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CardType.h"
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "ConSettings.generated.h"

/**
 * 
 */
UCLASS()
class MULTIGAME_API UConSettings : public USaveGame
{
	GENERATED_BODY()


public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
        uint32 RowSize;
    UPROPERTY(VisibleAnywhere, Category = Basic)
        uint32 TotalPairs;
    UPROPERTY(VisibleAnywhere, Category = Basic)
        ECardStyle CardStyle;

    UConSettings();
	
};
