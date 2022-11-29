#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "BPFL_Math.generated.h"

UCLASS()
class VIOLENTENDS_API UBPFL_Math : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Predict the aim rotation required to hit a moving target actor */
	UFUNCTION(BlueprintCallable)
	static FRotator PredictRotationToMovingActor(
		FVector Position, float ProjectileVelocity, FVector TargetPosition, FVector TargetVelocity);
};