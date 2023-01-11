#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

UENUM(BlueprintType)
enum class EItemCategory: uint8 {
    Ammo UMETA(DisplayName = "Ammo"),
    Equipment UMETA(DisplayName = "Equipment"),
    Consumables UMETA(DisplayName = "Consumables"),
    Misc UMETA(DisplayName = "Miscellaneous")
};

UENUM(BlueprintType)
enum class EItemRarity_ : uint8
{
    Common       UMETA(DisplayName = "Common"),  
    Uncommon     UMETA(DisplayName = "Uncommon"),
    Rare         UMETA(DisplayName = "Rare"),
    Epic         UMETA(DisplayName = "Epic"),
    Legendary    UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EItemTier_ : uint8
{
    None  = 0  UMETA(DisplayName = "None"),
    Tier1 = 1  UMETA(DisplayName = "Tier 1"),  
    Tier2 = 2  UMETA(DisplayName = "Tier 2"),
    Tier3 = 3  UMETA(DisplayName = "Tier 3"),
    Tier4 = 4  UMETA(DisplayName = "Tier 4"),
    Tier5 = 5  UMETA(DisplayName = "Tier 5"),
};