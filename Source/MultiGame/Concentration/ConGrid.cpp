// Fill out your copyright notice in the Description page of Project Settings.


#include "ConGrid.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <string>



#include "ConSettings.h"
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
	static ConstructorHelpers::FClassFinder<UConSettingsBaseUI> gameUISettingsRef(
		TEXT("/Game/Concentration/ConcentrationSettingsUI"));
	static ConstructorHelpers::FObjectFinderOptional<UParticleSystem> successPSysRef(
		TEXT("ParticleSystem'/Game/Concentration/SuccessEffect.SuccessEffect'"));

	
	FlipCurve = curveRef.Get();
	GameUIClass = gameUIRef.Class;
	GameSettingsUIClass = gameUISettingsRef.Class;
	SuccessParticleSystem = successPSysRef.Get();
	LoadTextures<ECardType>();
	LoadTextures<ECardStyle>();
}

template <class T>
void AConGrid::LoadTextures()
{
	ensure(TEXT("Invalid texture loading type"));
}

template <>
void AConGrid::LoadTextures<ECardType>()
{
	LoadTextures<ECardType>(ECardType::Values, ECardType::Path, cardTypeTextures);
}

template <>
void AConGrid::LoadTextures<ECardStyle>()
{
	LoadTextures<ECardStyle>(ECardStyle::Values, ECardStyle::Path, cardStyleTextures);
}

template <class T>
void AConGrid::LoadTextures(const WannabeEnum& values, FString Path, std::vector<UTexture2D*>& textureMap)
{
	textureMap = {};

	FString ResourceString;
	for (auto item : values)
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> texResource(
			*("Texture2D'"+ Path + item.first + "." + item.first + "'"));
		if (texResource.Succeeded())
		{
			textureMap.push_back(texResource.Object);
		}
		else
			textureMap.push_back(nullptr);
	}
}

void AConGrid::LoadMatchSettings()
{
	UConSettings* SettingsSave = Cast<UConSettings>(UGameplayStatics::LoadGameFromSlot("ConSettings", 0));
	if (SettingsSave && !ResetSavedSettings)
	{
		RowSize = SettingsSave->RowSize;
		TotalPairs = SettingsSave->TotalPairs;
		CardStyleIndex = SettingsSave->CardStyle;
	}
	else
	{
		SettingsSave = Cast<UConSettings>(UGameplayStatics::CreateSaveGameObject(UConSettings::StaticClass()));
		if (SettingsSave)
		{
			SettingsSave->RowSize = RowSize;
			SettingsSave->TotalPairs = TotalPairs;
			SettingsSave->CardStyle = CardStyleIndex;

			if (UGameplayStatics::SaveGameToSlot(SettingsSave, "ConSettings", 0))
			{
				//success
			}
		}
		else
		{
			//err
		}
	}


	MatchedPairs.clear();
	attempts = 0;
	HoldingCard = nullptr;
}

void AConGrid::GenerateCard(int32 row, int32 col)
{
	const FVector location = FVector(
		-row * BlockSpacing,
		col * BlockSpacing,
		0.f
	) + GetActorLocation();


	
	cards.push_back(
		GetWorld()->SpawnActor<AConCard>(location, FRotator(0, 0, 0)));

	if (cards.back() != nullptr)
	{
		cards.back()->Init(FlipCurve, this);
	}
	else
	{
		cards.pop_back();
	}
}

void AConGrid::SetCameraLocation(int32 maxRows)
{
	int32 col = RowSize < cards.size() ? RowSize : cards.size();
	float cameraDistanceX = col * CameraDistancePerCardX;
	float cameraDistanceY = maxRows * CameraDistancePerCardY;

	CameraRef->SetActorLocation(FVector(
		-(maxRows - 1) * 0.5f * BlockSpacing,
		(col - 1) * 0.5f * BlockSpacing,
		(cameraDistanceX > cameraDistanceY ? cameraDistanceX : cameraDistanceY) + CameraZOffset
	) + GetActorLocation());
}

void AConGrid::AssignTypesAndShuffle()
{
	std::vector<int32> enumIndexList;
	for (int i = 0; i < cardTypeTextures.size(); ++i)
		enumIndexList.push_back(i);

	if (!ensureMsgf(enumIndexList.size() >= cards.size() / 2, TEXT("Not enough pairs to match selected amount")))
	{
		return;
	}

	auto rng = std::default_random_engine{};
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(enumIndexList.begin(), enumIndexList.end(), rng);
	std::shuffle(cards.begin(), cards.end(), rng);

	UTexture2D* backTexture = GetTextureOfType<ECardStyle>(CardStyleIndex);

	for (int32 i = 0; i < TotalPairs; ++i)
	{
		UTexture2D* frontTexture = GetTextureOfType<ECardType>(enumIndexList[i]);
		cards[i]->SetType(enumIndexList[i], frontTexture, backTexture);
		cards[i + TotalPairs]->SetType(enumIndexList[i], frontTexture, backTexture);
	}
}

void AConGrid::StartupUI()
{
	if (gameUI)
	{
		gameUI->MatchQuit->SetVisibility(ESlateVisibility::Visible);
		gameUI->EndPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		gameUI = static_cast<UConBaseUI*>(CreateWidget(
			GetWorld()->GetFirstPlayerController(), GameUIClass, "Game UI"));
		gameUI->SettingsUIClass = GameSettingsUIClass;
		gameUI->AddToViewport();
		gameUI->gridRef = this;
	}
	gameUI->TotalPairs->SetText(FText::FromString(FString::FromInt(TotalPairs)));

}

// Called when the game starts or when spawned
void AConGrid::BeginPlay()
{
	Super::BeginPlay();

	StartMatch();
}

void AConGrid::StartMatch()
{
	LoadMatchSettings();

	if (cards.size())
	{
		for (auto Card : cards)
		{
			if (!Card->IsPendingKill()) Card->Destroy();
		}
	}
	
	cards.clear();

	int32 maxRows = FMath::CeilToInt((TotalPairs * 2.f) / RowSize);
	int32 col;
	for (int32 row = 0; row < maxRows; ++row)
	{
		for (col = 0; col < RowSize; ++col)
		{
			GenerateCard(row, col);
			
			if (cards.size() >= TotalPairs * 2)
			{
				row = cards.size();
				break;
			}
		}
	}

	SetCameraLocation(maxRows);

	AssignTypesAndShuffle();

	StartupUI();
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
template <class T>
UTexture2D* AConGrid::GetTextureOfType(int32 index)
{
	ensure(TEXT("Invalid texture type"));
}

template <>
UTexture2D* AConGrid::GetTextureOfType<ECardType>(int32 index)
{
	if (index >= cardTypeTextures.size()) return nullptr;
	return cardTypeTextures[index];
}

template <>
UTexture2D* AConGrid::GetTextureOfType<ECardStyle>(int32 index)
{
	if (index >= cardStyleTextures.size()) return nullptr;
	return cardStyleTextures[index];
}
