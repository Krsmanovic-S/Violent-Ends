#pragma once

#include "CoreMinimal.h"

#include "ViolentEnds/Item/ItemBase.h"
#include "EquipmentType.h"

#include "EquipmentBase.generated.h"

UCLASS()
class VIOLENTENDS_API UEquipmentBase : public UItemBase
{
	GENERATED_BODY()

public:

	UEquipmentBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	EEquipmentType EquipmentType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	EItemRarity_ EquipmentRarity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	EItemTier_ EquipmentTier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	TArray<FEquipmentStats> StatList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	FGameplayTag EquipmentTag;

	// Primary use: Equip item
	virtual bool TryUseItem_Implementation(AActor* Target) override;

	// Secondary use:
	virtual bool TryUseItemSecondary_Implementation(AActor* Target) override;

	// Always return true
	virtual bool CanBeUsed_Implementation(AActor* Target) override;

};