// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <vector>


#include "ConBaseUI.h"
#include "CoreMinimal.h"
#include "ConCard.h"
#include "Curves/CurveFloat.h"
#include "Engine/Texture2D.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Actor.h"
#include "ConGrid.generated.h"

UCLASS(minimalapi)
class AConGrid : public AActor
{
	GENERATED_BODY()

	/** Holding card for pair check*/
	AConCard* HoldingCard;

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;


	std::vector<ECardType> cardTypes;
	std::map<ECardType, UTexture2D*> cardTypeTextures;

	UConBaseUI* gameUI;
	
public:	
	// Sets default values for this actor's properties
	AConGrid();

	/** Successful pairs */
	std::vector<ECardType> MatchedPairs;

	/** Attempts*/
	int attempts = 0;

	/** Curve for card flipping animation*/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	UCurveFloat* FlipCurve;

	/** Curve for card flipping animation*/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UConBaseUI> GameUIClass;

	/** Match success particle system*/
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	UParticleSystem* SuccessParticleSystem;
	
    /** Number of cards on each grid row */
    UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
    int32 RowSize = 4;
	
	/** Number of pairs */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	int32 TotalPairs = 8;

	/** Spacing of blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	float BlockSpacing = 300;

private:
	void LoadTextures();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Handle the block being clicked */
	void MatchCards(AConCard& card);

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	UTexture2D* GetTextureOfType(ECardType type);

};
