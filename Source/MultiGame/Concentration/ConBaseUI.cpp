// Fill out your copyright notice in the Description page of Project Settings.


#include "ConBaseUI.h"

void UConBaseUI::NativeConstruct()
{
	Super::NativeConstruct();

	//simpleButton->OnClicked.AddDynamic(this, &UConBaseUI::SimpleButtonClicked);
}

void UConBaseUI::SimpleButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("The hardest button to button"));
}
