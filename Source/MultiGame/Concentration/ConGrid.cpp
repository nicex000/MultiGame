// Fill out your copyright notice in the Description page of Project Settings.


#include "ConGrid.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <string>



#include "ConstructorHelpers.h"
#include "Engine/World.h"

// Sets default values
AConGrid::AConGrid()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnRoot"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinderOptional<UCurveFloat> curveRef(
		TEXT("CurveFloat'/Game/Concentration/FlipCurve.FlipCurve'"));
	static ConstructorHelpers::FClassFinder<UConBaseUI> gameUIRef(
		TEXT("/Game/Concentration/ConcentrationMatchUI"));
	static ConstructorHelpers::FObjectFinderOptional<UParticleSystem> successPSysRef(
		TEXT("ParticleSystem'/Game/Concentration/SuccessEffect.SuccessEffect'"));

	
	FlipCurve = curveRef.Get();
	GameUIClass = gameUIRef.Class;
	SuccessParticleSystem = successPSysRef.Get();
	LoadTextures();
}

void AConGrid::LoadTextures()
{
	cardTypes = {};
	cardTypeTextures = {};
	for (int32 i = 0; i <= static_cast<int32>(ECardType::SCISSORS); ++i)
	{
		cardTypes.push_back(static_cast<ECardType>(i));
	}
	
	const FString prefix = "ECardType::";
	FString ResourceString;
	for (auto CardType : cardTypes)
	{
		ResourceString = StaticEnum<ECardType>()->GetValueAsString(CardType);
		ResourceString.RemoveFromStart(prefix);
		ConstructorHelpers::FObjectFinder<UTexture2D> texResource(
			*("Texture2D'/Game/Concentration/FrontFaces/"+ ResourceString + "." + ResourceString +"'"));
		if (texResource.Succeeded())
		{
			//using insert to make sure i don't overwrite anything
			cardTypeTextures.insert({ CardType, texResource.Object });
		}
	}
}

// Called when the game starts or when spawned
void AConGrid::BeginPlay()
{
	Super::BeginPlay();

	cards = {};
	int32 maxRows = FMath::CeilToInt((TotalPairs * 2.f) / RowSize);
	int32 col;
	for (int32 row = 0; row < maxRows; ++row)
	{
		for (col = 0; col < RowSize; ++col)
		{
			const FVector location = FVector(
				-row * BlockSpacing,
				col * BlockSpacing,
				0.f
			) + GetActorLocation();
			
			//mem leaks
			AConCard* newCard = GetWorld()->SpawnActor<AConCard>(location, FRotator(0, 0, 0));

			if (newCard != nullptr)
			{
				newCard->Init(FlipCurve, this);
			}
			cards.push_back(newCard);

			if (cards.size() >= TotalPairs*2)
			{
				row = cards.size();
				break;
			}
		}
	}
	col = RowSize < cards.size() ? RowSize : cards.size();
	float cameraDistanceX = col * CameraDistancePerCardX;
	float cameraDistanceY = maxRows * CameraDistancePerCardY;

	GetWorld()->GetFirstPlayerController()->GetViewTarget()->SetActorLocation(FVector(
		-(maxRows - 1) * 0.5f * BlockSpacing,
		(col - 1) * 0.5f * BlockSpacing,
		(cameraDistanceX > cameraDistanceY ? cameraDistanceX : cameraDistanceY) + CameraZOffset
	) + GetActorLocation());

	std::vector<ECardType> enumList = cardTypes;
	
	if (!ensureMsgf(enumList.size() >= cards.size() / 2, TEXT("Not enough pairs to match selected amount")))
	{
		return;
	}

	auto rng = std::default_random_engine{};
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(enumList.begin(), enumList.end(), rng);
	std::shuffle(cards.begin(), cards.end(), rng);

	for (int32 i = 0; i < TotalPairs; ++i)
	{
		UTexture2D* texture = GetTextureOfType(enumList[i]);
		cards[i]->SetType(enumList[i], texture);
		cards[i + TotalPairs]->SetType(enumList[i], texture);
	}
	
	gameUI = static_cast<UConBaseUI*>(CreateWidget(
		GetWorld()->GetFirstPlayerController(), GameUIClass, "Game UI"));
	gameUI->AddToViewport();
	gameUI->TotalPairs->SetText(FText::FromString(FString::FromInt(TotalPairs)));
}

void AConGrid::MatchCards(AConCard& card)
{
	if (HoldingCard == nullptr)
	{
		HoldingCard = &card;
		return;
	}

	attempts++;
	gameUI->Attempts->SetText(FText::FromString(FString::FromInt(attempts)));
	
	if (card.Type == HoldingCard->Type)
	{
		MatchedPairs.push_back(card.Type);
		gameUI->SuccessfulPairs->SetText(FText::FromString(FString::FromInt(MatchedPairs.size())));

		// pair success effect
		HoldingCard->SuccessEffect(SuccessParticleSystem);
		card.SuccessEffect(SuccessParticleSystem);
		
		if (MatchedPairs.size() >= TotalPairs)
		{
			//game success
			EndMatch(true);
			return;
		}
	}
	else
	{
		//pair failure
		HoldingCard->HideCard();
		card.HideCard();
	}
	
	HoldingCard = nullptr;
	
}

void AConGrid::EndMatch(bool didWin)
{
	FTimerHandle fallTimerHandle, UITimeHandle;
	GetWorldTimerManager().SetTimer(fallTimerHandle, [this]()
		{
			for (auto Card : cards)
			{
				Card->EnablePhysicsWithPush(ImpulseStrength, ImpulseOffset);
			}
		},
	1, false, VictoryFallDelay);
	GetWorldTimerManager().SetTimer(UITimeHandle, [this]()
		{
			gameUI->MatchQuit->SetVisibility(ESlateVisibility::Hidden);
			gameUI->EndPanel->SetVisibility(ESlateVisibility::Visible);
		},
		1, false, VictoryScreenDelay + VictoryFallDelay);
}

UTexture2D* AConGrid::GetTextureOfType(ECardType type)
{
	UTexture2D* texture = nullptr;
	if (cardTypeTextures.find(type) != cardTypeTextures.end())
	{
		return cardTypeTextures[type];
	}
	return texture;
}
