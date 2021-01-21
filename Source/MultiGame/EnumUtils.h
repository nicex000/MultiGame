// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MULTIGAME_API EnumUtils
{
public:
	EnumUtils();
	~EnumUtils();
	
	template<typename T>
	static FString EnumToString(const FString& enumName, const T value, const FString& defaultValue);
	static FString ExpandEnumString(const FString& name, const FString& enumName);
};
