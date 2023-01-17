#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEC_AimMovementSpeedCalculation.generated.h"

UCLASS()
class VIOLENTENDS_API UGEC_AimMovementSpeedCalculation : public UGameplayEffectExecutionCalculation {

	GENERATED_BODY()

public:

	UGEC_AimMovementSpeedCalculation();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};

