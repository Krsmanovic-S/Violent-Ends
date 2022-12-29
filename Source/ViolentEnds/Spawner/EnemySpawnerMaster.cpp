#include "EnemySpawnerMaster.h"

#include "../BaseEnemy.h"
#include "EnemySpawnerArea.h"

#if WITH_EDITORONLY_DATA
	#include "Components/BillboardComponent.h"
#endif // WITH_EDITORONLY_DATA

AEnemySpawnerMaster::AEnemySpawnerMaster()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	DisplayComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("DisplayComponent"));
	DisplayComponent->Sprite = DisplayTexture;
	DisplayComponent->SpriteInfo.Category = TEXT("Spawner");
	DisplayComponent->SpriteInfo.DisplayName = FText::FromName(GetFName());
	DisplayComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
#endif // WITH_EDITORONLY_DATA
}

bool AEnemySpawnerMaster::TrySpawnRandomEnemy()
{
	int32 Size = SpawnMapping.Num();
	if (Size < 1) { return false; }

	// Pick a random spawn class 
	TArray<TSubclassOf<ABaseEnemy>> SpawnClasses;
	SpawnMapping.GetKeys(SpawnClasses);

	TSubclassOf<ABaseEnemy> SpawnClassChosen = SpawnClasses[FMath::RandRange(0, Size - 1)];
	
	return TrySpawnRandomEnemyOfClass(SpawnClassChosen);
}

bool AEnemySpawnerMaster::TrySpawnRandomEnemyOfClass(TSubclassOf<ABaseEnemy> SpawnClass)
{
	if (!SpawnMapping.Find(SpawnClass)) { return false; }

	TArray<AEnemySpawnerArea*>& SpawnAreasRef = SpawnMapping[SpawnClass].SpawnerAreas;

	// Pick a random spawner to try to spawn the enemy
	float RandomChance = FMath::RandRange(0.f, 1.f);
	float CurrentChance = 0.f;
	AEnemySpawnerArea* SpawnerRef = nullptr;
	for (auto Item : SpawnAreasRef) {
		if (!Item || !SpawnClass) continue;

		CurrentChance += Item->SpawnChance[SpawnClass];
		if (CurrentChance >= RandomChance) {
			SpawnerRef = Item;
			break;
		}
	}
	if (!SpawnerRef) return false;

	bool Success = (SpawnerRef->TrySpawnRandomEnemyOfClass(SpawnClass));

#if WITH_EDITORONLY_DATA
	DebugMessage = SpawnerRef->DebugMessage;
#endif // WITH_EDITORONLY_DATA}
	return Success;
}

#if WITH_EDITOR
void AEnemySpawnerMaster::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	FName PropertyName = PropertyChangedEvent.Property->GetFName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawnerMaster, DisplayTexture)) {
		   DisplayComponent->Sprite = DisplayTexture;
	}
}

void AEnemySpawnerMaster::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	FName PropertyName = PropertyChangedEvent.Property->GetFName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawnerMaster, SpawnMapping)) {
		int32 TotalWeight;

		for (auto Item : SpawnMapping) {
			TSubclassOf<ABaseEnemy> SpawnClass = Item.Key;
			TotalWeight = 1;

			TArray<AEnemySpawnerArea*>& Spawners = Item.Value.SpawnerAreas;

			for (int32 i = 0; i < Spawners.Num(); i++)
			{
				TotalWeight += Spawners[i]->Weight;
			}

			for (int32 i = 0; i < Spawners.Num(); i++) {
				Spawners[i]->SpawnChance[SpawnClass] = Spawners[i]->Weight / TotalWeight;
			}
		}
	}

}


void AEnemySpawnerMaster::TESTING_SpawnRandomEnemy()
{
	DebugMessage = TEXT("");
	TrySpawnRandomEnemy();
}

void AEnemySpawnerMaster::TESTING_SpawnRandomEnemyOfClass()
{
	DebugMessage = TEXT("");
	TrySpawnRandomEnemyOfClass(TESTING_SpawnClass);
}
#endif // WITH_EDITOR
