#include "BPFL_Math.h"

#include "Kismet/KismetMathLibrary.h"

FRotator UBPFL_Math::PredictRotationToMovingActor(
	FVector Position, float ProjectileVelocity, FVector TargetPosition, FVector TargetVelocity)
{
	FVector OffsetToTarget = Position - TargetPosition;
	float ProjectileTravelTime = OffsetToTarget.Length() / ProjectileVelocity;

	// This is a naive approach, as target velocity will affect the projectile travel time
	FVector PredictedTargetLocation = TargetPosition + (TargetVelocity * ProjectileTravelTime);

	return UKismetMathLibrary::FindLookAtRotation(Position, PredictedTargetLocation);
}