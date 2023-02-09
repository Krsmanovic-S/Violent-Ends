#pragma once

#include "CoreMinimal.h"

#include "EquipmentType.h"
#include "ViolentEnds/Item/ItemBase.h"

#include "EquipmentBase.generated.h"

class UGameplayEffect;
/*
 * Base equipment class for the character
 * The equipment will have the tag of similar to : Item.Equipment.Gear.{xxx}GearType} (for non-weapons) and Item.Equipment.Weapon.{WeaponType} (for weapons)
 * Equipments will have a base gameplay effect that has a reference to a blueprint created GE that represents the default effect that will have on the character (Including attributes they modify and what abilities they grant)
 * Rarity grants equipments an array of struct of type FEquipmentStatList defines the type of attribute it modifies
 * When equipped, it will apply the base effect of the equipment with the corresponding tag of Character.Effect.Item.Equipment.{EquipmentType}.{EquipmentSubType}
 * When unequipped it will remove the effect(s) of said tag from the character
*/

UCLASS()
class VIOLENTENDS_API UEquipmentBase : public UItemBase
{
	GENERATED_BODY()

public:
	UEquipmentBase();

	/**
	 * The type of equipment this is
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	EEquipmentType EquipmentType;

	/**
	 * The rarity of this equipment
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	EItemRarity_ EquipmentRarity;

	/**
	 * The tier of this equipment
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	EItemTier_ EquipmentTier;

	/**
	 * The list of equipment stats that grants to the character when equipped
	 * Granted from rarity
	 * The effect will be made when the inventory successfully equip the item
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	TArray<FEquipmentStats> StatList;

	/**
	 * The base equipment stats
	 * Contains attribute modifiers as well as list of abilities
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Equipment")
	TSubclassOf<UGameplayEffect> EquipmentBaseStats;

	// Primary use: Equip item
	virtual bool TryUseItem_Implementation(AActor* Target) override;

	// Secondary use:
	virtual bool TryUseItemSecondary_Implementation(AActor* Target) override;

	// Always return true
	virtual bool CanBeUsed_Implementation(AActor* Target) override;

	private:
	static FGameplayTag GetEffectTagFromEquipmentTag(FGameplayTag EquipmentTag);
};