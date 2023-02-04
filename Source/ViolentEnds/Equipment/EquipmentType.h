#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "EquipmentType.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType: uint8 {
	Type        UMETA(DisplayName = "Type"),
 	Weapon      UMETA(DisplayName = "Weapon"),
    Helmet      UMETA(DisplayName = "Helmet"),
    BodyArmor   UMETA(DisplayName = "Body Armor"),
    Legs        UMETA(DisplayName = "Legs"),
    Boots       UMETA(DisplayName = "Boots"),
    Arms        UMETA(DisplayName = "Arms"),
};

USTRUCT(BlueprintType)
struct VIOLENTENDS_API FEquipmentStats
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayAttribute AttributeAffected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Magnitude;
	
};


USTRUCT(BlueprintType)
struct VIOLENTENDS_API FEquipmentEffectSpec
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FEquipmentStats> Stats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEquipmentType EquipmentType;
	
};