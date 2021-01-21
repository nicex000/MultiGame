// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MultiGamePlayerController.h"

AMultiGamePlayerController::AMultiGamePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMultiGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}
