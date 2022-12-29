#include "EnemySpawnerArea.h"

#include "../BaseEnemy.h"
#include "Components/BoxComponent.h"
#include "DynamicSpawnable.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
	#include "DrawDebugHelpers.h"
#endif // WITH_EDITOR

AEnemySpawnerArea::AEnemySpawnerArea()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetGenerateOverlapEvents(false);
	SpawnArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Ignore);
	SetRootComponent(SpawnArea);

	MaxAttempts = 5;
}

bool AEnemySpawnerArea::TrySpawnRandomEnemy()
{
#if WITH_EDITORONLY_DATA
	DebugMessage = TEXT("");
#endif // WITH_EDITORONLY_DATA
	if (EnemySpawnClasses.Num() < 1)
	{
#if WITH_EDITORONLY_DATA
		DebugMessage += (TEXT("Class list is empty"));

#endif // WITH_EDITORONLY_DATA
		return false;
	}
	TSubclassOf<ABaseEnemy> SpawnClass = EnemySpawnClasses[FMath::RandRange(0, EnemySpawnClasses.Num() - 1)];

	return TrySpawnRandomEnemyOfClass(SpawnClass);
}

bool AEnemySpawnerArea::TrySpawnRandomEnemyOfClass(TSubclassOf<ABaseEnemy> EnemyClass)
{
#if WITH_EDITORONLY_DATA
	DebugMessage = TEXT("");
#endif // WITH_EDITORONLY_DATA

	// Define Spawn Location
	FVector SpawnLocation = FVector(-INFINITY);
	FVector SpawnLocationOffset = FVector();

	// Define Spawn Rotation
	FRotator SpawnRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

	float TraceRadius = 10.f;

	// Use hit trace to get a spawning location on the ground
	FHitResult HitResult;
	ECollisionChannel TraceChannel =
		ECC_Visibility; // TODO: Convert to actual collision channel when new channels are added, refer to section
						// [/Script/Engine.CollisionProfile] of DefaultEngine.ini for channel selection

	auto EnemyClassDefaultObject = EnemyClass.GetDefaultObject();

	// Get the spawn dynamic properties
	if (auto SpawnDynamic = Cast<IDynamicSpawnable>(EnemyClassDefaultObject))
	{
		SpawnLocationOffset = SpawnDynamic->GetSpawnLocationOffset();
		if (SpawnDynamic->ShouldSpawnWithFixedRotation()) { SpawnRotation = SpawnDynamic->GetFixedSpawnRotation(); }

		TraceRadius = SpawnDynamic->GetSpawnRadius();
		TraceRadius > 0.f ? TraceRadius : 10.f;
	}
	// Define spawn randomization
	bool bIsValidRandomLocation = false;
	FVector RandomLocation;
	for (int32 j = 0; j < MaxAttempts; j++)
	{
		RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(
			SpawnArea->GetComponentLocation(), SpawnArea->GetScaledBoxExtent());
		bIsValidRandomLocation = true;
		for (int32 i = 0; i < PreviousSpawnLocations.Num(); i++)
		{
			//	Add 10% to the radius to avoid collision
			if (FVector::DistSquared2D(PreviousSpawnLocations[i], RandomLocation) < (TraceRadius * TraceRadius * 1.21f))
			{
				bIsValidRandomLocation = false;
				break;
			}
		}
		
		for (int32 i = 0; i < AvailableSpawnLocations.Num(); i++)
		{
			//	Add 10% to the radius to avoid collision
			if (FVector::DistSquared2D(AvailableSpawnLocations[i], RandomLocation) < (TraceRadius * TraceRadius * 1.21f))
			{
				bIsValidRandomLocation = false;
				break;
			}
		}

		if (bIsValidRandomLocation) { AvailableSpawnLocations.Add(RandomLocation); }
	}
	// Pick a random spawn location
	RandomLocation = AvailableSpawnLocations.Pop();

	// Set trace locations
	FVector TraceStartLocation = RandomLocation + FVector(0.f, 0.f, 2 * SpawnArea->GetScaledBoxExtent().Z);
	FVector TraceEndLocation = RandomLocation - FVector(0.f, 0.f, 2 * SpawnArea->GetScaledBoxExtent().Z);

	// Hit trace to determine if the spawn location is valid
	if (GetWorld()->SweepSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, FQuat::Identity, TraceChannel,
			FCollisionShape::MakeSphere(TraceRadius)))
	{
#if WITH_EDITOR
		DrawDebugSphere(GetWorld(), HitResult.Location, TraceRadius, 12, FColor::Purple, false, 15.f );
#endif // WITH_EDITOR

		SpawnLocation = HitResult.Location + SpawnLocationOffset - FVector(0.f, 0.f, TraceRadius);
		FActorSpawnParameters SpawnParams;

		ABaseEnemy* SpawnedEnemy =
			GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
		PreviousSpawnLocations.Add(SpawnedEnemy->GetActorLocation());
		// if (OnEnemyCreated.IsBound()) { OnEnemyCreated.Broadcast(SpawnedEnemy); }

#if WITH_EDITORONLY_DATA
		DebugMessage += (TEXT("Spawn Successful"));

#endif // WITH_EDITORONLY_DATA
		return true;
	}

#if WITH_EDITORONLY_DATA
	DebugMessage += (TEXT("Can't find a valid spawn Surface"));

#endif // WITH_EDITORONLY_DATA

	return false;
}
void AEnemySpawnerArea::PurgeSpawnLocationCache()
{
	PreviousSpawnLocations.Empty();
	AvailableSpawnLocations.Empty();
}
#if WITH_EDITOR

void AEnemySpawnerArea::OnConstruction(const FTransform& Transform) {}

void AEnemySpawnerArea::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = PropertyChangedEvent.Property->GetFName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawnerArea, SpawnerType))
	{
		AvailableSpawnLocations.Empty();
		switch (SpawnerType)
		{
			case ESpawnerType::PreAllocate:

				break;
			case ESpawnerType::Random_Guess:

				break;
			default:
				break;
		}
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawnerArea, PivotType))
	{
		FVector NewLocation;
		FVector Offset = FVector::ZeroVector;

		switch (PivotType)
		{
			case ESpawnerPivotType::Center:
				NewLocation = FVector::ZeroVector;
				break;
			case ESpawnerPivotType::TopLeft:
				Offset = SpawnArea->GetScaledBoxExtent() / 2;
				Offset.X *= 1;
				Offset.Y *= -1;
				Offset.Z = 0.f;
				break;
			case ESpawnerPivotType::TopRight:
				Offset = SpawnArea->GetScaledBoxExtent() / 2;
				Offset.X *= 1;
				Offset.Y *= 1;
				Offset.Z = 0.f;
				break;
			case ESpawnerPivotType::BottomLeft:
				Offset = SpawnArea->GetScaledBoxExtent() / 2;
				Offset.X *= -1;
				Offset.Y *= -1;
				Offset.Z = 0.f;
				break;
			case ESpawnerPivotType::BottomRight:
				Offset = SpawnArea->GetScaledBoxExtent() / 2;
				Offset.X *= -1;
				Offset.Y *= 1;
				Offset.Z = 0.f;
				break;
			default:
				break;
		}

		NewLocation = Offset;
		SpawnArea->SetRelativeLocation(NewLocation);

		UE_LOG(LogTemp, Display, TEXT("%s Offset by (%f, %f, %f) to (%f, %f, %f)"), *GetFName().ToString(),
			SpawnArea->GetRelativeLocation().X, SpawnArea->GetRelativeLocation().Y, SpawnArea->GetRelativeLocation().Z,
			SpawnArea->GetComponentLocation().X, SpawnArea->GetComponentLocation().Y,
			SpawnArea->GetComponentLocation().Z)
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawnerArea, bApplyScale))
	{
		SpawnArea->SetBoxExtent(SpawnerSize);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawnerArea, SpawnerSize))
	{
		if (bInstantUpdate) { SpawnArea->SetBoxExtent(SpawnerSize); }
	}
}
void AEnemySpawnerArea::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	FName PropertyName = PropertyChangedEvent.Property->GetFName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawnerArea, EnemySpawnClasses))
	{
		SpawnChance.Empty();

		for (auto Item : EnemySpawnClasses)
		{
			SpawnChance.Add(Item, 0.f);
		}
	}
}
void AEnemySpawnerArea::ApplyScaleAsSpawnExtend()
{
	FVector SpawnAreaSize = SpawnArea->GetUnscaledBoxExtent();
	FVector NewSpawnAreaSize = FVector(SpawnAreaSize.X * GetActorScale3D().X, SpawnAreaSize.Y * GetActorScale3D().Y,
		SpawnAreaSize.Z * GetActorScale3D().Z);
	SpawnArea->SetBoxExtent(NewSpawnAreaSize);

	// Reset scale to 1.0
	SetActorScale3D(FVector(1.0f));

	SpawnerSize = SpawnArea->GetScaledBoxExtent();
}

void AEnemySpawnerArea::TESTING_SpawnRandomEnemy()
{
	#if WITH_EDITORONLY_DATA

	DebugMessage = TEXT("");
	TrySpawnRandomEnemy();

	#endif // WITH_EDITORONLY_DATA
}

void AEnemySpawnerArea::TESTING_SpawnRandomEnemyOfClass()
{
	#if WITH_EDITORONLY_DATA

	DebugMessage = TEXT("");
	TrySpawnRandomEnemyOfClass(TESTING_SpawnClass);

	#endif // WITH_EDITORONLY_DATA
}

#endif // WITH_EDITOR