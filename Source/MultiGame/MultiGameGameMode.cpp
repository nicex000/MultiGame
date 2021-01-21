// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MultiGameGameMode.h"
#include "MultiGamePlayerController.h"
#include "MultiGamePawn.h"

AMultiGameGameMode::AMultiGameGameMode()
{
	// no pawn by default
	DefaultPawnClass = AMultiGamePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMultiGamePlayerController::StaticClass();
}
