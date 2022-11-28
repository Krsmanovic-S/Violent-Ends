#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemEnumsAndStructs.generated.h"

// -----------------------------------------------------------------
// Enum Classes
// -----------------------------------------------------------------
UENUM(BlueprintType)
enum class EItemType : uint8
{
    None        UMETA(DisplayName = "None"),
    Weapon      UMETA(DisplayName = "Weapon"),
    Helmet      UMETA(DisplayName = "Helmet"),
    BodyArmor   UMETA(DisplayName = "Body Armor"),
    Legs        UMETA(DisplayName = "Legs"),
    Boots       UMETA(DisplayName = "Boots"),
    Arms        UMETA(DisplayName = "Arms"),
    Consumable  UMETA(DisplayName = "Consumable"),
    Ammo        UMETA(DisplayName = "Ammo")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common       UMETA(DisplayName = "Common"),  
    Uncommon     UMETA(DisplayName = "Uncommon"),
    Rare         UMETA(DisplayName = "Rare"),
    Epic         UMETA(DisplayName = "Epic"),
    Legendary    UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EItemTier : uint8
{
    Tier1   UMETA(DisplayName = "Tier 1"),  
    Tier2   UMETA(DisplayName = "Tier 2"),
    Tier3   UMETA(DisplayName = "Tier 3"),
    Tier4   UMETA(DisplayName = "Tier 4"),
    Tier5   UMETA(DisplayName = "Tier 5"),
    Tier6   UMETA(DisplayName = "Tier 6"),
    Tier7   UMETA(DisplayName = "Tier 7") 
};

// -----------------------------------------------------------------
// Structs
// -----------------------------------------------------------------
USTRUCT(BlueprintType)
struct FItemStats
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Stats")
    TMap<TSubclassOf<class UBaseCustomDamageType>, float> ItemDamageTypes;

    //-------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    float ItemDamage = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    float ItemFireDamage = 0;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    float ItemCriticalChance = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    float ItemCriticalDamage = 0;
    //-------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    float ItemAddHealth = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    float ItemHealthRegen = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    float ItemArmor = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    float ItemFireResist = 0;
    //-------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mobility Stats")
    float ItemAddStamina = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mobility Stats")
    float ItemStaminaRegen = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mobility Stats")
    float ItemDashCost = 0;
    //-------------------------------------------------------------------
};

// This struct is used in blueprints to set base range values for items,
// and will interact with the item whenever it first appears in the world.
// Ranges have to be displayed as FVector2D since TTuple and TPair cannot
// be UPROPERTY() values. X value represents the minimum and Y the maximum.
USTRUCT(BlueprintType)
struct FItemStatsRange
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Stats")
    TMap<TSubclassOf<class UBaseCustomDamageType>, FVector2D> DamageTypesRange;

    //-------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    FVector2D DamageRange = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    FVector2D FireDamageRange = 0;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    FVector2D CriticalChanceRange = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Stats")
    FVector2D CriticalDamageRange = 0;
    //-------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    FVector2D HealthRange = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    FVector2D HealthRegenRange = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    FVector2D ArmorRange = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defense Stats")
    FVector2D FireResistRange = 0;
    //-------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mobility Stats")
    FVector2D StaminaRange = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mobility Stats")
    FVector2D StaminaRegenRange = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mobility Stats")
    FVector2D DashCostRange = 0;
    //-------------------------------------------------------------------
};

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UBaseItem> ItemClass;

    UPROPERTY(EditAnywhere)
    float ChanceToSpawn = 1.f;

    UPROPERTY(EditAnywhere)
    int32 MinimalPlayerLevel = 1;

    UPROPERTY(EditAnywhere)
    int32 MaximalPlayerLevel = 70;
};