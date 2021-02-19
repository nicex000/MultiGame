// Fill out your copyright notice in the Description page of Project Settings.


#include "ConBaseUI.h"
#include "ConGrid.h"

#include "Kismet/GameplayStatics.h"

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
	if (SettingsUI)
	{
		SettingsUI->SetVisibility(ESlateVisibility::Visible);
		SettingsUI->SetIsEnabled(true);
	}
	else if (SettingsUIClass)
	{
		SettingsUI = static_cast<UConSettingsBaseUI*>(CreateWidget(
			GetWorld()->GetFirstPlayerController(), SettingsUIClass, "Settings UI"));
		SettingsUI->AddToViewport(100);
		SettingsUI->CancelBtn->OnClicked.AddDynamic(this, &UConBaseUI::SettingsExitClicked);
	}
}

void UConBaseUI::SettingsExitClicked()
{
	SettingsUI->SetVisibility(ESlateVisibility::Collapsed);
	SettingsUI->SetIsEnabled(false);
}

void UConBaseUI::RestartButtonClicked()
{
	gridRef->StartMatch();
}
