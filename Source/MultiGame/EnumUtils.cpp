// Fill out your copyright notice in the Description page of Project Settings.


#include "EnumUtils.h"

#include "UObjectGlobals.h"

EnumUtils::EnumUtils()
{
}

EnumUtils::~EnumUtils()
{
}

template<typename T>
FString EnumUtils::EnumToString(const FString& enumName, const T value, const FString& defaultValue)
{
    UEnum* pEnum = FindObject<UEnum>(((UPackage*)-1), *enumName, true);
    return pEnum
        ? ExpandEnumString(pEnum->GetNameByIndex(static_cast<uint8>(value)).ToString(), enumName)
        : defaultValue;
}

FString EnumUtils::ExpandEnumString(const FString& name, const FString& enumName)
{
    FString expanded(name);
    FString spaceLetter("");
    FString spaceNumber("");
    FString search("");
    expanded.ReplaceInline(*enumName, TEXT(""), ESearchCase::CaseSensitive);
    expanded.ReplaceInline(TEXT("::"), TEXT(""), ESearchCase::CaseSensitive);
    for (TCHAR letter = 'A'; letter <= 'Z'; ++letter)
    {
        search = FString::Printf(TEXT("%c"), letter);
        spaceLetter = FString::Printf(TEXT(" %c"), letter);
        expanded.ReplaceInline(*search, *spaceLetter, ESearchCase::CaseSensitive);
    }
    for (TCHAR number = '0'; number <= '9'; ++number)
    {
        search = FString::Printf(TEXT("%c"), number);
        spaceNumber = FString::Printf(TEXT(" %c"), number);
        expanded.ReplaceInline(*search, *spaceNumber, ESearchCase::CaseSensitive);
    }
    expanded.ReplaceInline(TEXT("_"), TEXT(" -"), ESearchCase::CaseSensitive);
    expanded = expanded.RightChop(1).TrimStart().TrimEnd();
    return expanded;
}