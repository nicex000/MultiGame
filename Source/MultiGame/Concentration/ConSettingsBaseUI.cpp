// Fill out your copyright notice in the Description page of Project Settings.


#include "Concentration/ConSettingsBaseUI.h"

#include "ConSettings.h"

void UConSettingsBaseUI::NativeConstruct()
{
	ApplyBtn->OnClicked.AddDynamic(this, &UConSettingsBaseUI::ApplyClicked);
	
	UConSettings* SettingsSave = Cast<UConSettings>(UGameplayStatics::LoadGameFromSlot("ConSettings", 0));

	for (int32 i = 0; i < static_cast<int32>(ECardStyle::R7); i++)
		CardStylePicker->AddOption(StaticEnum<ECardStyle>()->GetEnumText(i).ToString());
	CardStylePicker->SetSelectedIndex(0);
	
	if (SettingsSave)
	{
		RowSizeSlider->SetValue(SettingsSave->RowSize);
		TotalPairsSlider->SetValue(SettingsSave->TotalPairs);
		CardStylePicker->SetSelectedIndex(static_cast<int32>(SettingsSave->CardStyle));
	}
}

void UConSettingsBaseUI::ApplyClicked()
{
	UConSettings* SettingsSave = Cast<UConSettings>(UGameplayStatics::CreateSaveGameObject(UConSettings::StaticClass()));
	if (SettingsSave)
	{
		SettingsSave->RowSize = RowSizeSlider->GetValue();
		SettingsSave->TotalPairs = TotalPairsSlider->GetValue();
		SettingsSave->CardStyle = static_cast<ECardStyle>(CardStylePicker->GetSelectedIndex());

		if (UGameplayStatics::SaveGameToSlot(SettingsSave, "ConSettings", 0))
		{
			//success
		}
	}
	else
	{
		//err
	}
	SetVisibility(ESlateVisibility::Collapsed);
	SetIsEnabled(false);
}