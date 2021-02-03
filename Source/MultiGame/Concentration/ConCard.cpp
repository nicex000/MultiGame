// Fill out your copyright notice in the Description page of Project Settings.


#include "ConCard.h"

#include "UObject/ConstructorHelpers.h"
#include "ConGrid.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AConCard::AConCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    struct FConstructionStatics
    {
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CardMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> CardBaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> CardBackMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> CardFrontMaterial;
    	FConstructionStatics() : CardMesh(TEXT("/Game/Concentration/CardMesh.CardMesh")),
			CardBaseMaterial(TEXT("/Game/Concentration/CardMaterial.CardMaterial")),
			CardBackMaterial(TEXT("/Game/Concentration/CardBackMat.CardBackMat")),
			CardFrontMaterial(TEXT("/Game/Concentration/CardFrontMat.CardFrontMat"))
    	{
    	}
    };
	static FConstructionStatics ConstructorStatics;
	CardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CardRoot"));
	RootComponent = CardRoot;
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
	if (ConstructorStatics.CardMesh.Succeeded())
	{
		CardMesh->SetStaticMesh(ConstructorStatics.CardMesh.Get());
		CardMesh->SetRelativeScale3D(FVector(1.f, 1.35f, 0.035f));
		CardMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
		
		baseMaterial = ConstructorStatics.CardBaseMaterial.Get();
		backMaterial = ConstructorStatics.CardBackMaterial.Get();
		frontMaterial = ConstructorStatics.CardFrontMaterial.Get();


		CardMesh->OnClicked.AddDynamic(this, &AConCard::CardClicked);
		CardMesh->OnInputTouchBegin.AddDynamic(this, &AConCard::OnFingerPressedBlock);
	}

	
}

void AConCard::BeginPlay()
{
	Super::BeginPlay();
	DynamicBaseMaterial = UMaterialInstanceDynamic::Create(baseMaterial,
		this, FName(TEXT("base mat")));
	DynamicBackMaterial = UMaterialInstanceDynamic::Create(backMaterial,
		this, FName(TEXT("back mat")));
	DynamicFrontMaterial = UMaterialInstanceDynamic::Create(frontMaterial,
		this, FName(TEXT("front mat")));
	CardMesh->SetMaterial(0, DynamicBaseMaterial);
	CardMesh->SetMaterial(1, DynamicBackMaterial);
	CardMesh->SetMaterial(2, DynamicFrontMaterial);

	CardMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RootComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void AConCard::Init(UCurveFloat* flipCurve, AConGrid* owner)
{
	FlipCurve = flipCurve;
	OwningGrid = owner;

	FOnTimelineFloat timelineCallback;
	FOnTimelineEventStatic timelineFinishedCallback;
	timelineCallback.BindUFunction(this, FName("FlipCard"));
	timelineFinishedCallback.BindUFunction(this, FName("TimelineRotateEnd"));
	if (FlipCurve) rotationTimeline.AddInterpFloat(FlipCurve, timelineCallback);
	rotationTimeline.SetTimelineFinishedFunc(timelineFinishedCallback);
}

void AConCard::SetType(ECardType type, UTexture2D* texture)
{
	Type = type;
	if (texture != nullptr)
	{
		DynamicFrontMaterial->SetTextureParameterValue("Tex", texture);
	}
}

void AConCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	rotationTimeline.TickTimeline(DeltaTime);
}



void AConCard::FlipCard()
{
	//animation
	curveRotValue = FlipCurve->GetFloatValue(rotationTimeline.GetPlaybackPosition());
	CardMesh->SetRelativeRotation(FRotator(180.f * curveRotValue, startingRotation.Yaw, startingRotation.Roll));
	
}

void AConCard::TimelineRotateEnd()
{
	if (rotationTimeline.GetPlaybackPosition() > 0 || !FlipCurve)
	{
		OwningGrid->MatchCards(*this);
		faceUp = true;
	}
	else
	{
		faceUp = false;
	}
}

void AConCard::CardClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AConCard::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AConCard::HandleClicked()
{
	// Do nothing if it's already face up or already in animation
	if (faceUp || rotationTimeline.IsPlaying())
	{
		return;
	}
	//timeline -> https://unrealcpp.com/open-door-with-timeline/ timeline finished callback
	startingRotation = CardMesh->GetRelativeRotation();
	rotationTimeline.PlayFromStart();
}

void AConCard::HideCard()
{
	//animation with timeline
	rotationTimeline.ReverseFromEnd();
}

void AConCard::Highlight(bool bOn)
{
	// Do not highlight if it's already face up.
	if (faceUp)
	{
		return;
	}

	if (bOn)
	{
		DynamicBackMaterial->SetScalarParameterValue("Opacity", 0.4f);
	}
	else
	{
		DynamicBackMaterial->SetScalarParameterValue("Opacity", 1.f);
	}
}

void AConCard::SuccessEffect(UParticleSystem* pSystem)
{
	auto pos = CardMesh->GetComponentLocation();
	pos.Y -= 1.f;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pSystem, pos);
	
}

void AConCard::EnablePhysicsWithPush(float impulseStrength, float impulseOffset)
{
	CardMesh->SetSimulatePhysics(true);
	FVector impulsePoint = CardMesh->GetComponentLocation();
	impulsePoint.X += (FMath::RandBool() ? 1.f : -1.f) * impulseOffset;
	impulsePoint.Y += (FMath::RandBool() ? 1.f : -1.f) * impulseOffset;
	CardMesh->AddImpulseAtLocation(FVector::DownVector * impulseStrength, impulsePoint);
}
