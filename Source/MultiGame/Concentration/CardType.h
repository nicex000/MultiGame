// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <utility>
#include <string>
#include <vector>

#include "CoreMinimal.h"
#include <Runtime\Engine\Classes\Engine\Blueprint.h>

/**
 * 
 */

typedef std::vector<std::pair<FString, FString>> WannabeEnum;

struct ECardType
{
	static const WannabeEnum Values;
	static const FString Path;
};

struct ECardStyle
{
	static const WannabeEnum Values;
	static const FString Path;
};

