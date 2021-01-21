// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ConPawn.generated.h"

UCLASS(config=Game)
class MULTIGAME_API AConPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		class AConCard* FocusedCard;

public:
	// Sets default values for this pawn's properties
	AConPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void OnResetVR();
	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

};
