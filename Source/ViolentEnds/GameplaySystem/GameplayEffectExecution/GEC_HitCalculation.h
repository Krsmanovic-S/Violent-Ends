#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEC_HitCalculation.generated.h"

UCLASS()
class VIOLENTENDS_API UGEC_HitCalculation : public UGameplayEffectExecutionCalculation {

	GENERATED_BODY()

public:

	UGEC_HitCalculation();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};

