// Fill out your copyright notice in the Description page of Project Settings.


#include "ConPawn.h"


#include "ConCard.h"
#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AConPawn::AConPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AConPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

// Called to bind functionality to input
void AConPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", EInputEvent::IE_Pressed, this, &AConPawn::OnResetVR);
	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &AConPawn::TriggerClick);
}

void AConPawn::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}


void AConPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AConPawn::TriggerClick()
{
	if (FocusedCard)
	{
		FocusedCard->HandleClicked();
	}
}

void AConPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
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
		AConCard* HitActor = Cast<AConCard>(HitResult.Actor.Get());
		if (FocusedCard != HitActor)
		{
			if (FocusedCard)
			{
				FocusedCard->Highlight(false);
			}
			if (HitActor)
			{
				HitActor->Highlight(true);
			}
			FocusedCard = HitActor;
		}
	}
	else if (FocusedCard)
	{
		FocusedCard->Highlight(false);
		FocusedCard = nullptr;
	}
}
