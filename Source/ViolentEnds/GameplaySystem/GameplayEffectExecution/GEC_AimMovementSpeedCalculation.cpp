#include "GEC_AimMovementSpeedCalculation.h"

#include "ViolentEnds/GameplaySystem/AttributeSet/AttributeSet_BaseAttribute.h"

#define CaptureAttribute(Exe,S, A, P) Exe.AttemptCalculateCapturedAttributeMagnitude(S.A##Def, P, A);

struct FMovementSpeedStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MovementSpeed)
	DECLARE_ATTRIBUTE_CAPTUREDEF(WalkSpeed)
	DECLARE_ATTRIBUTE_CAPTUREDEF(RunSpeed)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MoveSpeedWhileAiming)

	FMovementSpeedStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, MovementSpeed, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, WalkSpeed, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, RunSpeed, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_BaseAttribute, MoveSpeedWhileAiming, Source, false)
	}
};

static FMovementSpeedStatics MovementSpeedStatics() {
	static FMovementSpeedStatics MovementSpeedStatic;
	return MovementSpeedStatic;
}

UGEC_AimMovementSpeedCalculation::UGEC_AimMovementSpeedCalculation() 
{

	RelevantAttributesToCapture.Add(MovementSpeedStatics().MovementSpeedDef);
	RelevantAttributesToCapture.Add(MovementSpeedStatics().WalkSpeedDef);
	RelevantAttributesToCapture.Add(MovementSpeedStatics().RunSpeedDef);
	RelevantAttributesToCapture.Add(MovementSpeedStatics().MoveSpeedWhileAimingDef);
}


void UGEC_AimMovementSpeedCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FAggregatorEvaluateParameters EvaluationParams;

	float MovementSpeed(-1.f);
	float WalkSpeed(-1.f);
	float RunSpeed(-1.f);
	float MoveSpeedWhileAiming(-1.f);

	CaptureAttribute(ExecutionParams, MovementSpeedStatics(), MovementSpeed, EvaluationParams);
	CaptureAttribute(ExecutionParams, MovementSpeedStatics(), WalkSpeed, EvaluationParams);
	CaptureAttribute(ExecutionParams, MovementSpeedStatics(), RunSpeed, EvaluationParams);
	CaptureAttribute(ExecutionParams, MovementSpeedStatics(), MoveSpeedWhileAiming, EvaluationParams);

	UE_LOG(LogTemp, Display, TEXT("Captured MovementSpeed: %f"), MovementSpeed)
	UE_LOG(LogTemp, Display, TEXT("Captured WalkSpeed: %f"), WalkSpeed)
	UE_LOG(LogTemp, Display, TEXT("Captured RunSpeed: %f"), RunSpeed)
	UE_LOG(LogTemp, Display, TEXT("Captured MoveSpeedWhileAiming: %f"), MoveSpeedWhileAiming)


	UAbilitySystemComponent* TargetASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (!TargetASC) { return; }

	bool IsWalking =
		TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Status.Movement.Walk")));

	bool IsRunning =
		TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Status.Movement.Run")));

	bool IsAiming =
		TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Status.Movement.Aim")));

	float NewMovementSpeed = MovementSpeed;
	if (IsAiming) {
		// Return to walk mode
		TargetASC->AddMinimalReplicationGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Status.Movement.Walk")));
		NewMovementSpeed = WalkSpeed;
	}
	else {
		// Apply aim mode
		TargetASC->AddMinimalReplicationGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Status.Movement.Aim")));
		NewMovementSpeed = MoveSpeedWhileAiming;
	}

	// Add modifiers to execute
	FGameplayModifierEvaluatedData Data;
	//Data.Attribute = Stats.HealthDef.AttributeToCapture;
	Data.Attribute = UAttributeSet_BaseAttribute::GetMovementSpeedAttribute();
	Data.ModifierOp = EGameplayModOp::Override;
	Data.Magnitude = NewMovementSpeed;
	
	UE_LOG(LogTemp, Display, TEXT("New MovementSpeed: %f"), NewMovementSpeed)
	OutExecutionOutput.AddOutputModifier(Data);
}
