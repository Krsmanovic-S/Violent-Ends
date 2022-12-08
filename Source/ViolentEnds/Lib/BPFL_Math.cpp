#include "BPFL_Math.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


FRotator UBPFL_Math::PredictRotationToMovingActor(
	FVector Position, float ProjectileVelocity, FVector TargetPosition, FVector TargetVelocity)
{
	FVector OffsetToTarget = TargetPosition - Position;
	float ProjectileTravelTime = OffsetToTarget.Length() / ProjectileVelocity;

	// This is a naive approach, as target velocity will affect the projectile travel time
	FVector PredictedTargetLocation = TargetPosition + (TargetVelocity * ProjectileTravelTime);

	return UKismetMathLibrary::FindLookAtRotation(Position, PredictedTargetLocation);
}

FVector UBPFL_Math::BoxTraceForSpawnLocation(
	AActor* CallingActor, const FVector StartingPoint, const FVector StartingOffset, const FVector NotFoundOffset,
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTrace, const float ZHalfMeshSize, const FVector BoxHalfSize, const FVector2D RandomRange)
{
	FVector RandomUnitVector;
	FVector DirectionVector = FVector::ZeroVector;
	float RandomDistance = 0;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CallingActor);

	FHitResult DummyHitResult;
	FVector EndPoint = FVector::ZeroVector;

	bool FoundHit;

	for(int i = 0; i < 1; i++)
	{
		// Get a random direction around the caller and 
		// zero out the Z since we only care about X and Y.
		RandomUnitVector = FMath::VRand();
		RandomUnitVector *= FVector(1, 1, 0);

		DirectionVector = UKismetMathLibrary::GetDirectionUnitVector(FVector(0, 0, 0), RandomUnitVector);
		RandomDistance = FMath::RandRange(RandomRange.X, RandomRange.Y);

		// End is in line with the start just a certain distance away
		// in a randomly generated direction around the caller.
		EndPoint = StartingPoint + StartingOffset + (DirectionVector * RandomDistance);

		FoundHit = UKismetSystemLibrary::BoxTraceSingleForObjects(
			CallingActor->GetWorld(), StartingPoint + StartingOffset,
			EndPoint, BoxHalfSize, FRotator(0, 0, 0), ObjectsToTrace,
			false, ActorsToIgnore, EDrawDebugTrace::None, DummyHitResult, true
		);

		// Not hitting anything means we exit the loop
		// since we have found our end point location.
		if(!FoundHit) { break; }
	}

	// If we didn't find any suitable place we should return
	// the starting location with the not found offset.
	if(FoundHit) return StartingPoint + NotFoundOffset;

	// Line trace from above the end point to below it, this is used to get the location
	// of the floor which we need to determine where the final location will be.
	UKismetSystemLibrary::LineTraceSingle(CallingActor->GetWorld(), FVector(EndPoint.X, EndPoint.Y, 100), FVector(EndPoint.X, EndPoint.Y, -100),
	UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, DummyHitResult, true);

	// Adding the ZHalfMeshSize here so that the object reaches the ground on point.
	return FVector(EndPoint.X, EndPoint.Y, DummyHitResult.Location.Z + ZHalfMeshSize);
}