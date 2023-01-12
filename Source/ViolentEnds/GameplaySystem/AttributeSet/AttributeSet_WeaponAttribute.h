#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_WeaponAttribute.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class VIOLENTENDS_API UAttributeSet_WeaponAttribute: public UAttributeSet {

	GENERATED_BODY()

public:

	UAttributeSet_WeaponAttribute();

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData PhysicalWeaponDamageBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, PhysicalWeaponDamageBase)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData FireWeaponDamageBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, FireWeaponDamageBase)
	
	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData AttackRateBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, AttackRateBase)
		
	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData CritChanceBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, CritChanceBase)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData CritMultiplierBase;
	ATTRIBUTE_ACCESSORS(UAttributeSet_WeaponAttribute, CritMultiplierBase)

};