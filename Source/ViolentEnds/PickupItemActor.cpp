#include "PickupItemActor.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseItem.h"
#include "ItemEnumsAndStructs.h"
#include "BaseCustomDamageType.h"
#include "Components/WidgetComponent.h"


APickupItemActor::APickupItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// In the future change this to only contain the mesh.
	this->CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	RootComponent = this->CollisionBox;

	this->SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	this->SkeletalMeshComp->SetupAttachment(this->CollisionBox);

	this->ItemWidgetName =CreateDefaultSubobject<UWidgetComponent>(TEXT("Item Name Widget"));
	this->ItemWidgetName->SetupAttachment(this->SkeletalMeshComp);
}

void APickupItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	this->ItemWidgetName->SetVisibility(false);
}

void APickupItemActor::InitializePickupProperties()
{
	if(this->ContainedItem)
	{
		this->ContainedItem->World = GEngine->GameViewport->GetWorld();

		this->SkeletalMeshComp->SetSkeletalMesh(this->ContainedItem->PickupMesh);

		if(!this->bWasItemInitialized)
		{
			this->RandomizeItemStats();
		}
	}
}

FVector2D APickupItemActor::StatsRangeOnTier()
{
	FVector2D Result = FVector2D::ZeroVector;

	if(this->ContainedItem->MaximalTierAvailable == EItemTier::None ||
	   this->ContainedItem->MinimalTierAvailable == EItemTier::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Minimal or Maximal Tier not specified for Item.")); 
		return Result;
	}

	float PercentageIncrement = 0;

	switch(this->ContainedItem->MaximalTierAvailable - this->ContainedItem->MinimalTierAvailable)
	{
		case 1: PercentageIncrement = 50; break;
		case 2: PercentageIncrement = 33.33; break;
		case 3: PercentageIncrement = 25; break;
		case 4: PercentageIncrement = 20; break;
		default: UE_LOG(LogTemp, Warning, TEXT("Tier difference unknown, PickupActor error..")); return Result;
	}

	// How far are we from the minimal tier sets our lower bound for the range.
	switch(this->ContainedItem->Tier - this->ContainedItem->MinimalTierAvailable)
	{
		case 0: Result.X = 0; break;
		case 1: Result.X = PercentageIncrement; break;
		case 2: Result.X = PercentageIncrement * 2; break;
		case 3: Result.X = PercentageIncrement * 3; break;
		case 4: Result.X = PercentageIncrement * 4; break;
		default: UE_LOG(LogTemp, Warning, TEXT("Lower bound unknown, PickupActor error..")); return Result;	
	}

	// How far are we from the maximal tier sets our upper bound for the range.
	switch(this->ContainedItem->MaximalTierAvailable - this->ContainedItem->Tier)
	{
		case 0: Result.Y = 100; break;
		case 1: Result.Y = 100 - PercentageIncrement; break;
		case 2: Result.Y = 100 - PercentageIncrement * 2; break;
		case 3: Result.Y = 100 - PercentageIncrement * 3; break;
		case 4: Result.Y = 100 - PercentageIncrement * 4; break;
		default: UE_LOG(LogTemp, Warning, TEXT("Upper bound unknown, PickupActor error..")); return Result;		
	}

	Result.X /= 100;
	Result.Y /= 100;

	return Result;
}

FVector2D APickupItemActor::StatsRangeOnRarity()
{
	FVector2D Result = FVector2D::ZeroVector;

	switch(this->ContainedItem->Rarity)
	{
		case EItemRarity::Common:    Result.X = 0; Result.Y = 0.2; break;
		case EItemRarity::Uncommon:  Result.X = 0.2; Result.Y = 0.4; break;
		case EItemRarity::Rare: 	 Result.X = 0.4; Result.Y = 0.6; break;
		case EItemRarity::Epic: 	 Result.X = 0.6; Result.Y = 0.8; break;
		case EItemRarity::Legendary: Result.X = 0.8; Result.Y = 1; break;
		default: UE_LOG(LogTemp, Warning, TEXT("Rarity not set. PickupActor Error.")); return Result;
	}
	return Result;
}

void APickupItemActor::RandomizeIndividualStat(float& CurrentStat, FVector2D CurrentRange, const FVector2D& TierRange, const FVector2D& RarityRange)
{
	// Don't set anything if the range is 0.
	if(CurrentRange.X == 0 && CurrentRange.Y == 0) { return; }

	if(TierRange.X != 0) { CurrentRange.X = CurrentRange.Y * TierRange.X; }
	if(TierRange.Y != 1) { CurrentRange.Y *= TierRange.Y; }

	if(CurrentRange.X >= CurrentRange.Y) { UE_LOG(LogTemp, Warning, TEXT("Ranges are messed up because of TIER..")); }

	if(RarityRange.X != 0) { CurrentRange.X = CurrentRange.Y * RarityRange.X; }
	if(RarityRange.Y != 1) { CurrentRange.Y *= RarityRange.Y; }

	if(CurrentRange.X >= CurrentRange.Y) { UE_LOG(LogTemp, Warning, TEXT("Ranges are messed up because of RARITY..")); }

	CurrentStat = FMath::RandRange(CurrentRange.X, CurrentRange.Y);
	CurrentStat = FMath::Floor(CurrentStat);
}

void APickupItemActor::RandomizeItemStats()
{
	FVector2D TierStatRange = this->StatsRangeOnTier();
	FVector2D RarityStatRange = this->StatsRangeOnRarity();
	
	auto& Stats = this->ContainedItem->ItemStats;
	auto& StatsRange = this->ContainedItem->ItemStatsRange;

	// Initialize each damage type that exists in StatsRange.
	for(auto& DamageType : StatsRange.DamageTypesRange)
	{
		Stats.ItemDamageTypes.Add(DamageType.Key);

		this->RandomizeIndividualStat(Stats.ItemDamageTypes[DamageType.Key], StatsRange.DamageTypesRange[DamageType.Key], TierStatRange, RarityStatRange);
	}

	// Damage based stats.
	this->RandomizeIndividualStat(Stats.ItemDamage, StatsRange.DamageRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemFireDamage, StatsRange.FireDamageRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemCriticalChance, StatsRange.CriticalChanceRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemCriticalDamage, StatsRange.CriticalDamageRange, TierStatRange, RarityStatRange);

	// Defense based stats.
	this->RandomizeIndividualStat(Stats.ItemAddHealth, StatsRange.HealthRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemHealthRegen, StatsRange.HealthRegenRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemArmor, StatsRange.ArmorRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemFireResist, StatsRange.FireResistRange, TierStatRange, RarityStatRange);

	// Mobility based stats.
	this->RandomizeIndividualStat(Stats.ItemAddStamina, StatsRange.StaminaRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemStaminaRegen, StatsRange.StaminaRegenRange, TierStatRange, RarityStatRange);
	this->RandomizeIndividualStat(Stats.ItemDashCost, StatsRange.DashCostRange, TierStatRange, RarityStatRange);
}
