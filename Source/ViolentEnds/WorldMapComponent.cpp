#include "WorldMapComponent.h"

#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "ViolentEnds/LogMacros.h"

UWorldMapComponent::UWorldMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWorldMapComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> TargetPoints;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), this->TargetPointClass, TargetPoints);

	if (TargetPoints.Num() == 2)
	{
		this->UpperLeftCorner = TargetPoints[0];
		this->LowerRightCorner = TargetPoints[1];
	}
	else { LOG_WARNING(LogTemp, "Target Points Not Set Correctly"); }
}

float UWorldMapComponent::GetMapHeight() const
{
	if (this->LowerRightCorner != nullptr && this->UpperLeftCorner != nullptr)
	{
		return FMath::Abs(this->LowerRightCorner->GetActorLocation().X - this->UpperLeftCorner->GetActorLocation().X);
	}

	return 0.0;
}

float UWorldMapComponent::GetMapWidth() const
{
	if (this->LowerRightCorner != nullptr && this->UpperLeftCorner != nullptr)
	{
		return FMath::Abs(this->LowerRightCorner->GetActorLocation().Y - this->UpperLeftCorner->GetActorLocation().Y);
	}

	return 0.0;
}

FVector2D UWorldMapComponent::ConvertLocationToMapLocation(FVector InputLocation)
{
	if (this->UpperLeftCorner != nullptr)
	{
		FVector2D ResultingLocation;

		ResultingLocation.X = FMath::Abs(InputLocation.X - this->UpperLeftCorner->GetActorLocation().X);
		ResultingLocation.Y = FMath::Abs(InputLocation.Y - this->UpperLeftCorner->GetActorLocation().Y);

		return ResultingLocation;
	}

	return FVector2D(0, 0);
}
