// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "EnemySpawnerArea.generated.h"

class ABaseEnemy;
class UBoxComponent;

/**
 * Actor responsible for placng spawn zones for the enemies to spawn
 * Should only have 1 enemy class but can support multiple different ones
 * NOTE: If there are more than class then the random spawn function will spawn an enemy with even odds
 */
UCLASS()
class VIOLENTENDS_API AEnemySpawnerArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawnerArea();

	/**
	 * The area that determines random spawn
	 */
	UPROPERTY()
	UBoxComponent* SpawnArea;

	/**
	 * Array of enemy classes that can be created in this spawner
	 * Can have multiple enemy classes but they share spawning weight
	 * Should use only one if needed to have different spawning weight
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner")
	TArray<TSubclassOf<ABaseEnemy>> EnemySpawnClasses;

	/**
	 * The odds of a spawn command to call the spawn function on this area
	 * All classes share the same spawn weight
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner")
	int32 Weight;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool TrySpawnRandomEnemy();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool TrySpawnRandomEnemyOfClass(TSubclassOf<ABaseEnemy> EnemyClass);

#if WITH_EDITOR

	virtual void OnConstruction(const FTransform& Transform) override;

	/**
	 * Apply the scale of the actor to set the extends of the spawn box
	 * Avoid the works of tweaking the box extends property in editor
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spawner|Design")
	void ApplyScaleAsSpawnExtend();
#endif

	/// TESTING/DEBUGGING
#if WITH_EDITORONLY_DATA

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Spawner|Testing")
	FString DebugMessage;

#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spawner|Testing")
	void SpawnRandomEnemy();

#endif
};
