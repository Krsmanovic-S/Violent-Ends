#include "EnemySpawnerArea.h"

#include "../BaseEnemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEnemySpawnerArea::AEnemySpawnerArea()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SetRootComponent(SpawnArea);
}

bool AEnemySpawnerArea::TrySpawnRandomEnemy()
{
	if (EnemySpawnClasses.Num() < 1) { return false; }
	TSubclassOf<ABaseEnemy> SpawnClass = EnemySpawnClasses[FMath::RandRange(0, EnemySpawnClasses.Num() - 1)];

	return TrySpawnRandomEnemyOfClass(SpawnClass);
}

bool AEnemySpawnerArea::TrySpawnRandomEnemyOfClass(TSubclassOf<ABaseEnemy> EnemyClass)
{
	FVector SpawnLocation = FVector(-INFINITY);
	FRotator SpawnRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

	FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(
		SpawnArea->GetComponentLocation(), SpawnArea->GetScaledBoxExtent());

	FVector TraceStartLocation = RandomLocation;
	TraceStartLocation.Z = SpawnArea->GetComponentLocation().Z + SpawnArea->GetScaledBoxExtent().Z;
	FVector TraceEndLocation = RandomLocation - FVector(0.f, 0.f, SpawnArea->GetScaledBoxExtent().Z);
	// Use hit trace to get a spawning location on the ground
	FHitResult HitResult;
	ECollisionChannel TraceChannel =
		ECC_Visibility; // TODO: Convert to actual collision channel when new channels are added, refer to section
						// [/Script/Engine.CollisionProfile] of DefaultEngine.ini for channel selection

	if (GetWorld()->LineTraceSingleByChannel(
			HitResult, RandomLocation, RandomLocation + FVector(0.f, 0.f, -1000.f), TraceChannel))
	{
		SpawnLocation = HitResult.Location;
		FActorSpawnParameters SpawnParams;

		ABaseEnemy* SpawnedEnemy =
			GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass, SpawnLocation, FRotator(), SpawnParams);

		// if (OnEnemyCreated.IsBound()) { OnEnemyCreated.Broadcast(SpawnedEnemy); }

		return true;
	}

	return false;
}
#if WITH_EDITOR

void AEnemySpawnerArea::OnConstruction(const FTransform& Transform) {}

void AEnemySpawnerArea::ApplyScaleAsSpawnExtend()
{
	FVector SpawnAreaSize = SpawnArea->GetUnscaledBoxExtent();
	FVector NewSpawnAreaSize = FVector(SpawnAreaSize.X * GetActorScale3D().X, SpawnAreaSize.Y * GetActorScale3D().Y,
		SpawnAreaSize.Z * GetActorScale3D().Z);
	SpawnArea->SetBoxExtent(NewSpawnAreaSize);

	// Reset scale to 1.0
	SetActorScale3D(FVector(1.0f));
}

void AEnemySpawnerArea::SpawnRandomEnemy()
{
	bool Success = TrySpawnRandomEnemy();
	#if WITH_EDITORONLY_DATA

	if (Success) { DebugMessage = FString(TEXT("SPAWN SUCCESS")); }
	else { DebugMessage = FString(TEXT("SPAWN FAILED")); }

	#endif // WITH_EDITORONLY_DATA
}

#endif // WITH_EDITOR