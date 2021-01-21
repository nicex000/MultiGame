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