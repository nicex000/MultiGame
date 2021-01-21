// Fill out your copyright notice in the Description page of Project Settings.


#include "ConGameMode.h"


#include "ConPawn.h"
#include "MultiGamePlayerController.h"
#include "UserWidget.h"

AConGameMode::AConGameMode()
{
	// no pawn by default
	DefaultPawnClass = AConPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMultiGamePlayerController::StaticClass();
}

void AConGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AConGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
