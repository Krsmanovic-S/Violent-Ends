#include "PickupItemActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
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

	this->StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	this->StaticMeshComp->SetupAttachment(this->CollisionBox);

	this->ItemWidgetName =CreateDefaultSubobject<UWidgetComponent>(TEXT("Item Name Widget"));
	this->ItemWidgetName->SetupAttachment(this->StaticMeshComp);
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

		this->StaticMeshComp->SetStaticMesh(this->ContainedItem->PickupMesh);
		this->StaticMeshComp->SetWorldScale3D(FVector(0.5, 0.5, 0.5));

		if(!this->bWasItemInitialized)
		{
			this->RandomizeItemStats();
		}
	}
}

void APickupItemActor::RandomizeIndividualStat(float& CurrentStat, FVector2D CurrentRange)
{
	// Don't set anything if the range is 0.
	if(CurrentRange.X == 0 && CurrentRange.Y == 0)
	{
		return;
	}

	CurrentStat = FMath::RandRange(CurrentRange.X, CurrentRange.Y);

	CurrentStat = FMath::Floor(CurrentStat);
}

void APickupItemActor::RandomizeItemStats()
{
	EItemRarity Rarity = this->ContainedItem->Rarity;

	// Has no functionality right now.
	switch(Rarity)
	{
		case EItemRarity::Common:
			break;
		case EItemRarity::Uncommon:
			break;
		case EItemRarity::Rare:
			break;
		case EItemRarity::Epic:
			break;
		case EItemRarity::Legendary:
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Rarity not set. PickupActor Error."));
			return;
	}

	EItemTier Tier = this->ContainedItem->Tier;

	// Has no functionality right now.
	switch(Tier)
	{
		case EItemTier::Tier1:
			break;
		case EItemTier::Tier2:
			break;
		case EItemTier::Tier3:
			break;
		case EItemTier::Tier4:
			break;
		case EItemTier::Tier5:
			break;
		case EItemTier::Tier6:
			break;
		case EItemTier::Tier7:
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Tier not set. PickupActor Error."));
			return;
	}

	auto& Stats = this->ContainedItem->ItemStats;
	auto& StatsRange = this->ContainedItem->ItemStatsRange;

	// Initialize each damage type that exists in StatsRange.
	for(auto& DamageType : StatsRange.DamageTypesRange)
	{
		Stats.ItemDamageTypes.Add(DamageType.Key);

		this->RandomizeIndividualStat(Stats.ItemDamageTypes[DamageType.Key], StatsRange.DamageTypesRange[DamageType.Key]);
	}

	this->RandomizeIndividualStat(Stats.ItemDamage, StatsRange.DamageRange);
	this->RandomizeIndividualStat(Stats.ItemFireDamage, StatsRange.FireDamageRange);
	this->RandomizeIndividualStat(Stats.ItemCriticalChance, StatsRange.CriticalChanceRange);
	this->RandomizeIndividualStat(Stats.ItemCriticalDamage, StatsRange.CriticalDamageRange);

	this->RandomizeIndividualStat(Stats.ItemAddHealth, StatsRange.HealthRange);
	this->RandomizeIndividualStat(Stats.ItemHealthRegen, StatsRange.HealthRegenRange);
	this->RandomizeIndividualStat(Stats.ItemArmor, StatsRange.ArmorRange);
	this->RandomizeIndividualStat(Stats.ItemFireResist, StatsRange.FireResistRange);

	this->RandomizeIndividualStat(Stats.ItemAddStamina, StatsRange.StaminaRange);
	this->RandomizeIndividualStat(Stats.ItemStaminaRegen, StatsRange.StaminaRegenRange);
	this->RandomizeIndividualStat(Stats.ItemDashCost, StatsRange.DashCostRange);
}
