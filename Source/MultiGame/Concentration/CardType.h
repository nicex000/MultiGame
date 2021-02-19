// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime\Engine\Classes\Engine\Blueprint.h>

/**
 * 
 */
UENUM(BlueprintType)
enum class ECardType : uint8
{
	BACKPACK UMETA(DisplayName = "BACKPACK"),
	CRAYON UMETA(DisplayName = "CRAYON"),
	ERASER UMETA(DisplayName = "ERASER"),
	GLOBE UMETA(DisplayName = "GLOBE"),
	GLUE UMETA(DisplayName = "GLUE"),
	MARKER UMETA(DisplayName = "MARKER"),
	PENCIL UMETA(DisplayName = "PENCIL"),
	RULER UMETA(DisplayName = "RULER"),
	SCISSORS UMETA(DisplayName = "SCISSORS"),
};

UENUM(BlueprintType)
enum class ECardStyle : uint8
{
	B1 UMETA(DisplayName = "Blue 1"),
	B2 UMETA(DisplayName = "Blue 2"),
	B3 UMETA(DisplayName = "Blue 3"),
	R1 UMETA(DisplayName = "Red 1"),
	R2 UMETA(DisplayName = "Red 2"),
	R3 UMETA(DisplayName = "Red 3"),
	R4 UMETA(DisplayName = "Red 4"),
	R5 UMETA(DisplayName = "Red 5"),
	R6 UMETA(DisplayName = "Red 6"),
	R7 UMETA(DisplayName = "Red 7"),
};

