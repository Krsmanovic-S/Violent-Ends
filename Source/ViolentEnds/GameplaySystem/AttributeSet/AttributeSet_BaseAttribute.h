#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "AttributeSet_BaseAttribute.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class VIOLENTENDS_API UAttributeSet_BaseAttribute : public UAttributeSet
{
	GENERATED_BODY()

public:

	UAttributeSet_BaseAttribute();

	/**
	 * The current Health attribute of the associated character
	 * If it reaches 0 then the associated character is considered dead
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, Health)

	/**
	 * The max Health attribute of the character
	 * Current Health stat is limited by max Health
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Health")
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, HealthMax)

	/**
	 * The health regeneration attribute of the character
	 * Health is recovered by this amount per second
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Health")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, HealthRegen)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, Stamina)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData StaminaMax;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaMax)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaRegen)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Defense")
	FGameplayAttributeData Armour;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, Armour)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Defense")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, FireResistance)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Defense")
	FGameplayAttributeData ExplosiveResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, ExplosiveResistance)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData HealthMaxModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, HealthMaxModifier)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData StaminaMaxModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaMaxModifier)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, DamageReduction)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData HealthRegenModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, HealthRegenModifier)

	/**
	 *
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData StaminaRegenModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaRegenModifier)

	/**
	 * The current movement speed of the associated character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Health")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, MovementSpeed)

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void AdjustAttributeProportional(FGameplayAttributeData& Attribute, FGameplayAttributeData& AttributeMax, float NewValue);
};