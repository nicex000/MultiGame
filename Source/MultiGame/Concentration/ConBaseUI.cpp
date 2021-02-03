// Fill out your copyright notice in the Description page of Project Settings.


#include "ConBaseUI.h"

void UConBaseUI::NativeConstruct()
{
	Super::NativeConstruct();

	MatchQuit->OnClicked.AddDynamic(this, &UConBaseUI::QuitButtonClicked);
	EndQuit->OnClicked.AddDynamic(this, &UConBaseUI::QuitButtonClicked);
	EndSettings->OnClicked.AddDynamic(this, &UConBaseUI::SettingsButtonClicked);
	EndRestart->OnClicked.AddDynamic(this, &UConBaseUI::RestartButtonClicked);
}

void UConBaseUI::QuitButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("i give up :) come back when there's a main menu"));
}

void UConBaseUI::SettingsButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("still gotta do the settings ui :)"));
}

void UConBaseUI::RestartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("restart or something, but first lemme fix them mem ~~leaks~~ leeks"));
}
