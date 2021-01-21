// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MultiGamePawn.h"
#include "MultiGameBlock.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AMultiGamePawn::AMultiGamePawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMultiGamePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UCameraComponent* OurCamera = PC->GetViewTarget()->FindComponentByClass<UCameraComponent>())
			{
				FVector Start = OurCamera->GetComponentLocation();
				FVector End = Start + (OurCamera->GetComponentRotation().Vector() * 8000.0f);
				TraceForBlock(Start, End, true);
			}
		}
		else
		{
			FVector Start, Dir, End;
			PC->DeprojectMousePositionToWorld(Start, Dir);
			End = Start + (Dir * 8000.0f);
			TraceForBlock(Start, End, false);
		}
	}
}

void AMultiGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", EInputEvent::IE_Pressed, this, &AMultiGamePawn::OnResetVR);
	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &AMultiGamePawn::TriggerClick);
}

void AMultiGamePawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void AMultiGamePawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMultiGamePawn::TriggerClick()
{
	if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}
}

void AMultiGamePawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		AMultiGameBlock* HitBlock = Cast<AMultiGameBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}