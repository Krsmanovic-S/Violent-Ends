#include "GE_Aim.h"
#include "ViolentEnds/GameplaySystem/AttributeSet/AttributeSet_BaseAttribute.h"
#include "ViolentEnds/GameplaySystem/GameplayEffectExecution/GEC_AimMovementSpeedCalculation.h"


UGE_Aim::UGE_Aim() {
	//InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Character.Self.Aim")));
	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UAttributeSet_BaseAttribute::GetMovementSpeedAttribute();
	Modifier.Magnitude.SetValue (500.f);
	Modifier.ModifierOp = EGameplayModOp::Override;
	
	Modifiers.Add(Modifier);

	FGameplayEffectExecutionDefinition ExecDef;
	ExecDef.CalculationClass = UGEC_AimMovementSpeedCalculation::StaticClass();
	Executions.Add(ExecDef);
}