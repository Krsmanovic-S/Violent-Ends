#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "AttributeSet_WeaponAttribute.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class VIOLENTENDS_API UAttributeSet_WeaponAttribute : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSet_WeaponAttribute();

#pragma region Weapon_Stats_Base
	/**
	 * The amount of base Physical damage the current weapon can deal
	 * Base value
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData PhysicalWeaponDamageBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, PhysicalWeaponDamageBase)

	/**
	 * The amount of base Fire damage the current weapon can deal
	 * Base value
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData FireWeaponDamageBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, FireWeaponDamageBase)

	/**
	 * Attack rate of the current weapon (delay until next weapon fire)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData AttackRateBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, AttackRateBase)

	/**
	 * The crit chance of current weapon
	 * Base value
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData CritChanceBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, CritChanceBase)

	/**
	 * The crit multiplier of the current weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData CritMultiplierBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, CritMultiplierBase)

	/**
	 * The maximum distance offset in aim mode
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData AimDistanceBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, AimDistanceBase)

	/**
	 * The maximum level of zoom the weapon can have in aim mode
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData ZoomLevelBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, ZoomLevelBase)
#pragma endregion
#pragma region Weapon_Stats_Increase

	/**
	 * The modifying value of Physical damage of the current weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData PhysicalWeaponDamageIncreased;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, PhysicalWeaponDamageIncreased)

	/**
	 *  The modifying value of Fire damage of the current weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData FireWeaponDamageIncreased;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, FireWeaponDamageIncreased)

	/**
	 * The modifying value of attack rate of the current weapon (delay until next weapon fire)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData AttackRateIncreased;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, AttackRateIncreased)

	/**
	 * The modifying value of crit chance of current weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData CritChanceIncreased;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, CritChanceIncreased)

	/**
	 * The modifying value of crit multiplier of the current weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData CritMultiplierIncreased;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, CritMultiplierIncreased)

	/**
	 * The modifying value of maximum distance offset in aim mode
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData AimDistanceIncreased;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, AimDistanceIncreased)

	/**
	 * The modifying value of maximum level of zoom the weapon can have in aim mode
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData ZoomLevelIncreased;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, ZoomLevelIncreased)

#pragma endregion

protected:

};