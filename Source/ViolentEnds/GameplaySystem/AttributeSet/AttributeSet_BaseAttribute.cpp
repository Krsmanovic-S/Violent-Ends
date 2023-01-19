#include "AttributeSet_BaseAttribute.h"

#include "GameplayEffectExtension.h"

UAttributeSet_BaseAttribute::UAttributeSet_BaseAttribute()
{
	Health = 1.f;
	HealthMax = 1.f;
	HealthRegen = 0.f;

	Stamina = 1.f;
	StaminaMax = 1.f;
	StaminaRegen = 0.f;

	Armour = 1.f;
	FireResistance = 1.f;
	ExplosiveResistance = 1.f;

	HealthMaxModifier = 1.f;
	StaminaMaxModifier = 1.f;

	DamageReduction = 0.f;

	HealthRegenModifier = 1.f;
	StaminaRegenModifier = 1.f;

	WalkSpeed = 600.f;
	RunSpeed = 800.f;
	MoveSpeedWhileAiming = 400.f;
	MovementSpeed = WalkSpeed;
}

void UAttributeSet_BaseAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayAttribute Attribute = Data.EvaluatedData.Attribute;
	float NewValue = Data.EvaluatedData.Magnitude;

	if (Attribute == GetHealthMaxAttribute()) { 
		
		AdjustAttributeProportional(Health, HealthMax, NewValue); 
	

	}

	if (Attribute == GetStaminaMaxAttribute()) { 
		
		AdjustAttributeProportional(Stamina, StaminaMax, NewValue); }

	if (Attribute == GetHealthAttribute())
	{
		float ValidHealth = FMath::Clamp(NewValue, 0.f, GetHealthMax());
		SetHealth(ValidHealth);
	}
	if (Attribute == GetStaminaAttribute())
	{
		float ValidStamina = FMath::Clamp(NewValue, 0.f, GetStaminaMax());
		SetHealth(ValidStamina);
	}
}

void UAttributeSet_BaseAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}
void UAttributeSet_BaseAttribute::AdjustAttributeProportional(
	FGameplayAttributeData& Attribute, FGameplayAttributeData& AttributeMax, float NewValue)
{
	const float CurrentMax = AttributeMax.GetCurrentValue();
	const float Current = Attribute.GetCurrentValue();
	if (FMath::IsNearlyEqual(CurrentMax, 0.f))
	{
		Attribute.SetCurrentValue(0.f);
		return;
	}
	const float CurrentPercentage = Current / CurrentMax;
	if (!FMath::IsNearlyEqual(CurrentMax, NewValue))
	{
		AttributeMax.SetCurrentValue(NewValue);
		Attribute.SetCurrentValue(NewValue * CurrentPercentage);
	}
}
