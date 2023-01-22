#include "GEC_HitCalculation.h"

#include "ViolentEnds/GameplaySystem/AttributeSet/AttributeSet_BaseAttribute.h"
#include "ViolentEnds/GameplaySystem/AttributeSet/AttributeSet_WeaponAttribute.h"

#define CaptureAttribute(Exe,S, A, P) Exe.AttemptCalculateCapturedAttributeMagnitude(S.A##Def, P, A);

struct FDamageStatistics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthRegen);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaRegen);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armour);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExplosiveResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthMaxModifier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaMaxModifier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthRegenModifier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaRegenModifier);

	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalWeaponDamageBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireWeaponDamageBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChanceBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritMultiplierBase);

	FDamageStatistics()
	{
		// Capture the target's defense values
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, HealthMax, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, Stamina, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, StaminaMax, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, Armour, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, ExplosiveResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, DamageReduction, Target, false);

		// Capture the source's offensive values
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_WeaponAttribute, PhysicalWeaponDamageBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_WeaponAttribute, FireWeaponDamageBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_WeaponAttribute, CritChanceBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_WeaponAttribute, CritMultiplierBase, Source, false);
	}
};

static FDamageStatistics DamageStatistics()
{
	static FDamageStatistics Statics;
	return Statics;
}

UGEC_HitCalculation::UGEC_HitCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatistics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatistics().HealthMaxDef);
	RelevantAttributesToCapture.Add(DamageStatistics().HealthRegenDef);
	RelevantAttributesToCapture.Add(DamageStatistics().StaminaDef);
	RelevantAttributesToCapture.Add(DamageStatistics().StaminaMaxDef);
	RelevantAttributesToCapture.Add(DamageStatistics().StaminaRegenDef);
	RelevantAttributesToCapture.Add(DamageStatistics().ArmourDef);
	RelevantAttributesToCapture.Add(DamageStatistics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatistics().ExplosiveResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatistics().HealthMaxModifierDef);
	RelevantAttributesToCapture.Add(DamageStatistics().StaminaMaxModifierDef);
	RelevantAttributesToCapture.Add(DamageStatistics().DamageReductionDef);
	RelevantAttributesToCapture.Add(DamageStatistics().HealthRegenModifierDef);
	RelevantAttributesToCapture.Add(DamageStatistics().StaminaRegenModifierDef);

	RelevantAttributesToCapture.Add(DamageStatistics().PhysicalWeaponDamageBaseDef);
	RelevantAttributesToCapture.Add(DamageStatistics().FireWeaponDamageBaseDef);
	RelevantAttributesToCapture.Add(DamageStatistics().CritChanceBaseDef);
	RelevantAttributesToCapture.Add(DamageStatistics().CritMultiplierBaseDef);
}

void UGEC_HitCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	FAggregatorEvaluateParameters EvaluationParams;

	float Health = 0.f;
	float HealthMax = 0.f;
	float HealthRegen = 0.f;
	float Stamina = 0.f;
	float StaminaMax = 0.f;
	float StaminaRegen = 0.f;
	float Armour = 0.f;
	float FireResistance = 0.f;
	float ExplosiveResistance = 0.f;
	float HealthMaxModifier = 0.f;
	float StaminaMaxModifier = 0.f;
	float DamageReduction = 0.f;
	float HealthRegenModifier = 0.f;
	float StaminaRegenModifier = 0.f;

	float PhysicalWeaponDamageBase = 0.f;
	float FireWeaponDamageBase = 0.f;
	float CritChanceBase = 0.f;
	float CritMultiplierBase = 1.f;

	CaptureAttribute(ExecutionParams, DamageStatistics(), Health, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), HealthMax, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), HealthRegen, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), Stamina, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), StaminaMax, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), StaminaRegen, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), Armour, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), FireResistance, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), ExplosiveResistance, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), HealthMaxModifier, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), StaminaMaxModifier, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), DamageReduction, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), HealthRegenModifier, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), StaminaRegenModifier, EvaluationParams);

	CaptureAttribute(ExecutionParams, DamageStatistics(), PhysicalWeaponDamageBase, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), FireWeaponDamageBase, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), CritChanceBase, EvaluationParams);
	CaptureAttribute(ExecutionParams, DamageStatistics(), CritMultiplierBase, EvaluationParams);

	// Critical hit calculation
	bool bIsCritHit = (FMath::RandRange(0.f, 1.f) >= CritChanceBase);
	float FinalDamage = PhysicalWeaponDamageBase + FireWeaponDamageBase;
	if (bIsCritHit) { FinalDamage *= CritMultiplierBase; }

	// Armour and damage reduction calculation

	// Add modifiers to execute
	FGameplayModifierEvaluatedData Data;
	// Data.Attribute = Stats.HealthDef.AttributeToCapture;
	Data.Attribute = UAttributeSet_BaseAttribute::GetHealthAttribute();
	Data.ModifierOp = EGameplayModOp::Additive;
	Data.Magnitude = -FinalDamage;

	OutExecutionOutput.AddOutputModifier(Data);
}
