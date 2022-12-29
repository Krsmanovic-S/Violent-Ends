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

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner|Editor")
	UBillboardComponent* DisplayComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner|Editor")
	UTexture2D* DisplayTexture;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;

#endif // WITH_EDITOR

	/// TESTING/DEBUGGING
#if WITH_EDITORONLY_DATA

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Spawner|Testing")
	FString DebugMessage;

	/**
	 * The class used to try to spawn the enemy
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Spawner|Testing")
	TSubclassOf<ABaseEnemy> TESTING_SpawnClass;

#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR

	/**
	 * EDITOR ONLY
	 * Calls TrySpawnRandomEnemy
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spawner|Testing")
	void TESTING_SpawnRandomEnemy();

	/**
	 * EDITOR ONLY
	 * Calls TrySpawnRandomEnemyOfClass with TESTING_SpawnClass as parameter
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spawner|Testing")
	void TESTING_SpawnRandomEnemyOfClass();

#endif
};
