#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL_Math.generated.h"

UCLASS()
class VIOLENTENDS_API UBPFL_Math : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Predict the aim rotation required to hit a moving target actor */
	UFUNCTION(BlueprintCallable)
	static FRotator PredictRotationToMovingActor(
		FVector Position, float ProjectileVelocity, FVector TargetPosition, FVector TargetVelocity);

	/** Find a suitable location for any object that desires a drop effect on spawn */
	UFUNCTION(BlueprintCallable)
	static FVector BoxTraceForSpawnLocation(
		AActor* CallingActor, const FVector StartingPoint, const FVector StartingOffset, const FVector NotFoundOffset,
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTrace, const float ZHalfMeshSize, const FVector BoxHalfSize, const FVector2D RandomRange);
};