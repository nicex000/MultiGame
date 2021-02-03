// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardType.h"
#include "Components/TimelineComponent.h"
#include "ConCard.generated.h"

UCLASS(minimalapi)
class AConCard : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CardRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CardMesh;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicBaseMaterial;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicBackMaterial;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicFrontMaterial;

	UMaterialInterface* baseMaterial;
	UMaterialInterface* backMaterial;
	UMaterialInterface* frontMaterial;


	bool faceUp = false;

	FTimeline rotationTimeline;

	FRotator startingRotation;
	float curveRotValue;

public:
	// Sets default values for this actor's properties
	AConCard();
	
	UPROPERTY()
		ECardType Type;

	/** Grid that owns us */
	UPROPERTY()
		class AConGrid* OwningGrid;

	UPROPERTY()
		UCurveFloat* FlipCurve;

private:
	UFUNCTION()
	void FlipCard();

	UFUNCTION()
	void TimelineRotateEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	void Init(UCurveFloat* flipCurve, AConGrid* owner);
	void SetType(ECardType type, UTexture2D* texture);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** Handle the block being clicked */
	UFUNCTION()
	void CardClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();

	void HideCard();

	void Highlight(bool bOn);

	void SuccessEffect(UParticleSystem* pSystem);

	void EnablePhysicsWithPush(float impulseStregth, float impulseOffset);
};
