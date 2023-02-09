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
	 * The current Stamina attribute of the character
	 * Uses to activate abilities
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, Stamina)

	/**
	 * The max Stamina attribute of the character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData StaminaMax;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaMax)

	/**
	 * The health regeneration attribute of the character
	 * Health is recovered by this amount per second
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Stamina")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaRegen)

	/**
	 * The current movement speed of the character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|MovementSpeed")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, MovementSpeed)

	/**
	 * The walk speed of the character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|MovementSpeed")
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, WalkSpeed)

	/**
	 * The run speed of the character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|MovementSpeed")
	FGameplayAttributeData RunSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, RunSpeed)

	/**
	 * The movement speed of the character while in aim mode
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|MovementSpeed")
	FGameplayAttributeData MoveSpeedWhileAiming;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, MoveSpeedWhileAiming)

#pragma region Defensive_Values

	/**
	 * Damage taken multiplier
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, DamageReduction)

	/**
	 * The armour of the character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Defense")
	FGameplayAttributeData Armour;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, Armour)

	/**
	 * The fire resistance of the character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Defense")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, FireResistance)

	/**
	 * The explosive resistance of the character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Defense")
	FGameplayAttributeData ExplosiveResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, ExplosiveResistance)

#pragma endregion
#pragma region Additional_Attributes

	/**
	 * The max Health modifier
	 * Multiplicative to the base modifier(s)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData HealthMaxModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, HealthMaxModifier)

	/**
	 * The max Stamina modifier
	 * Multiplicative to the base modifier(s)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData StaminaMaxModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaMaxModifier)

	/**
	 * The Health regeneration modifier
	 * Multiplicative to the base modifier(s)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData HealthRegenModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, HealthRegenModifier)

	/**
	 * The Stamina regeneration modifier
	 * Multiplicative to the base modifier(s)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|Modifier")
	FGameplayAttributeData StaminaRegenModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, StaminaRegenModifier)

	/**
	 * The walk speed modifier
	 * Multiplicative to the base modifier(s)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|MovementSpeed")
	FGameplayAttributeData WalkSpeedModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, WalkSpeedModifier)

	/**
	 * The run speed modifier
	 * Multiplicative to the base modifier(s)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|MovementSpeed")
	FGameplayAttributeData RunSpeedModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, RunSpeedModifier)

	/**
	 * The aim move speed modifier
	 * Multiplicative to the base modifier(s)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay|Attributes|MovementSpeed")
	FGameplayAttributeData MoveSpeedWhileAimingModifier;
	ATTRIBUTE_ACCESSORS(UAttributeSet_BaseAttribute, MoveSpeedWhileAimingModifier)

#pragma endregion

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void AdjustAttributeProportional(FGameplayAttributeData& Attribute,
		FGameplayAttributeData& AttributeMax, float NewValue);
};