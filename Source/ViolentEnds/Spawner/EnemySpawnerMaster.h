// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnerMaster.generated.h"

class ABaseEnemy;
class AEnemySpawnerArea;

USTRUCT(BlueprintType)
struct VIOLENTENDS_API FAreaSpawner
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner")
	TArray<AEnemySpawnerArea*> SpawnerAreas;
};

UCLASS()
class VIOLENTENDS_API AEnemySpawnerMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnerMaster();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner")
	TMap<TSubclassOf<ABaseEnemy>, FAreaSpawner> SpawnMapping;

	/**
	* Attempt to spawn a random enemy of any class registered to the SpawnMapping
	* @return true if spawn successfully, false if failed to spawn 
	*/
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool TrySpawnRandomEnemy();

	/**
	* Attempt to spawn a random enemy of a specified class 
	* @return true if spawn successfully, false if failed to spawn
	*/
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool TrySpawnRandomEnemyOfClass(TSubclassOf<ABaseEnemy> SpawnClass);

};
