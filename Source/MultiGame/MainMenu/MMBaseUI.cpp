// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MMBaseUI.h"



#include "Concentration/ConSettingsBaseUI.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMMBaseUI::NativeConstruct()
{
	Super::NativeConstruct();

	QuitBtn->OnClicked.AddDynamic(this, &UMMBaseUI::QuitButtonClicked);
	ConStart->OnClicked.AddDynamic(this, &UMMBaseUI::ConcentrationStartClicked);
	ConSettings->OnClicked.AddDynamic(this, &UMMBaseUI::ConcentrationSettingsClicked);
	RailHeroStart->OnClicked.AddDynamic(this, &UMMBaseUI::RailHeroStartClicked);
	RailHeroSettings->OnClicked.AddDynamic(this, &UMMBaseUI::RailHeroSettingsClicked);

	conSettingsUI = nullptr;
}

void UMMBaseUI::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UMMBaseUI::ConcentrationStartClicked()
{
	UGameplayStatics::OpenLevel((UObject*)GetGameInstance(), FName(TEXT("ConcentrationMap")));
}

void UMMBaseUI::ConcentrationSettingsClicked()
{
	if (conSettingsUI)
	{
		conSettingsUI->SetVisibility(ESlateVisibility::Visible);
		conSettingsUI->SetIsEnabled(true);
	}
	else if (ConSettingsUIClass)
	{
		conSettingsUI = static_cast<UConSettingsBaseUI*>(CreateWidget(
			GetWorld()->GetFirstPlayerController(), ConSettingsUIClass, "Concentration Settings UI"));
		conSettingsUI->AddToViewport(100);
		conSettingsUI->CancelBtn->OnClicked.AddDynamic(this, &UMMBaseUI::ConcentrationSettingsExited);
	}
}

void UMMBaseUI::ConcentrationSettingsExited()
{
	conSettingsUI->SetVisibility(ESlateVisibility::Collapsed);
	conSettingsUI->SetIsEnabled(false);
}

void UMMBaseUI::RailHeroStartClicked()
{
	UGameplayStatics::OpenLevel((UObject*)GetGameInstance(), FName(TEXT("RailwayHeroMap")));
}

void UMMBaseUI::RailHeroSettingsClicked()
{
}

void UMMBaseUI::RailHeroSettingsExited()
{
}
