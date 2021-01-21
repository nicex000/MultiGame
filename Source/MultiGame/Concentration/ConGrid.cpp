// Fill out your copyright notice in the Description page of Project Settings.


#include "ConGrid.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <string>



#include "ConstructorHelpers.h"
#include "ResourceManager.h"
#include "Engine/World.h"

// Sets default values
AConGrid::AConGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinderOptional<UCurveFloat> curveRef(
		TEXT("CurveFloat'/Game/Concentration/FlipCurve.FlipCurve'"));
	static ConstructorHelpers::FClassFinder<UConBaseUI> gameUIRef(
		TEXT("/Game/Concentration/ConcentrationMatchUI"));
	
	FlipCurve = curveRef.Get();
	GameUIClass = gameUIRef.Class;
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


	std::vector<AConCard*> cards;
	
	for (int32 row = 0; row < FMath::CeilToInt((TotalPairs*2.f) / RowSize); ++row)
	{
		for (int32 col = 0; col < RowSize; ++col)
		{
			const FVector location = FVector(
				-row * BlockSpacing,
				col * BlockSpacing,
				0.f
			);
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
	
	// using fmath rand
	/*
	int randomFirst = FMath::RandRange(0, cards.size());
	int randomSecond = randomFirst;
	for (int retry = 0; retry < 10; ++retry)
	{
		randomSecond = FMath::RandRange(0, cards.size());
		if (randomSecond != randomFirst)
		{
			break;
		}
	}
	
	if (randomSecond == randomFirst)
	{
		if (ensureMsgf(cards.size() <= 1, TEXT("Issue creating cards, they weren't enough to make full pairs")))
		{
			randomFirst = 0;
			randomSecond = 1;
		}
	}

	cards[randomFirst]->Type = enumList[enumList.size() - 1];
	cards[randomSecond]->Type = cards[randomFirst]->Type;
	
	enumList.pop_back();
	cards.erase(cards.begin() + randomFirst);
	cards.erase(cards.begin() + randomSecond);*/

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

		if (MatchedPairs.size() >= TotalPairs)
		{
			//game success
			return;
		}
		// pair success effect
		HoldingCard->SuccessEffect();
		card.SuccessEffect();
	}
	else
	{
		//pair failure
		HoldingCard->HideCard();
		card.HideCard();
	}
	
	HoldingCard = nullptr;
	
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