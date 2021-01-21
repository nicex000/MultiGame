// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiGamePlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AMultiGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMultiGamePlayerController();
	void BeginPlay() override;
};


