#include "EnemySpawnerMaster.h"

#include "EnemySpawnerArea.h"
#include "../BaseEnemy.h"

AEnemySpawnerMaster::AEnemySpawnerMaster()
{
	PrimaryActorTick.bCanEverTick = false;

}

bool AEnemySpawnerMaster::TrySpawnRandomEnemy()
{
	int32 Size = SpawnMapping.Num();
	if (Size<1) return false;

	TArray<TSubclassOf<ABaseEnemy>> SpawnClasses;
	SpawnMapping.GetKeys(SpawnClasses);

	TSubclassOf<ABaseEnemy> SpawnClassChosen = SpawnClasses[FMath::RandRange(0, Size - 1)];
	

	return TrySpawnRandomEnemyOfClass(SpawnClassChosen);
}

bool AEnemySpawnerMaster::TrySpawnRandomEnemyOfClass(TSubclassOf<ABaseEnemy> SpawnClass)
{
	if (!SpawnMapping.Find(SpawnClass)) {return false;}

	TArray<AEnemySpawnerArea*>& SpawnAreasRef = SpawnMapping[SpawnClass].SpawnerAreas;

	AEnemySpawnerArea*& SpawnerRef = SpawnAreasRef[FMath::RandRange(0, SpawnAreasRef.Num() - 1)];
	return (SpawnerRef->TrySpawnRandomEnemyOfClass(SpawnClass));
}

