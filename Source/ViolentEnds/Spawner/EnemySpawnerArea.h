// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "EnemySpawnerArea.generated.h"

class ABaseEnemy;
class UBoxComponent;

UENUM()
enum class ESpawnerType : uint8
{
	PreAllocate UMETA(DisplayName = "PreAllocate", Tooltip = ""),
	Random_Guess UMETA(DisplayName = "RandomGuess", Tooltip = "")
};

UENUM()
enum class ESpawnerPivotType : uint8
{
	Center,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,

};

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
	 * Array of locations chosen to spawn actors
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Spawner")
	TArray<FVector> PreviousSpawnLocations;

	/**
	 * Array of locations chosen to spawn actors
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Spawner")
	TArray<FVector> AvailableSpawnLocations;

	/**
	 * The odds of a spawn command to call the spawn function on this area
	 * All classes share the same spawn weight
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner")
	int32 Weight;

	/**
	 * The chance of a random spawn from master to chose this spawner
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner")
	TMap<TSubclassOf<ABaseEnemy>, float> SpawnChance;

	/**
	 * The amount of attempts to take before giving up on trying to find a place to spawn, default = 5
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner")
	int32 MaxAttempts;

	/**
	 * Try to spawn a random actor of random class from EnemySpawnClasses array
	 * @return Spawned successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool TrySpawnRandomEnemy();

	/**
	 * Try to spawn a random actor of specified class
	 * @return Spawned successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool TrySpawnRandomEnemyOfClass(TSubclassOf<ABaseEnemy> EnemyClass);

	/**
	 * Clear the previous spawn locations cache
	 * Used to spawn a new batch
	 */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void PurgeSpawnLocationCache();

#if WITH_EDITORONLY_DATA

	/**
	 * Change the spawner type of this spawner
	 * PreAllocate: The spawner will perform a sphere trace across the spawn location and add them to a list of
	 * available spawn location(s) RandomGuess: Will not create a list of available location(s), instead this will run a
	 * check only when the spawn function(s) is called, this needs an attemp limit so if no spawn location(s) available
	 * it will fail instead of looping random locations forever
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner|Editor")
	ESpawnerType SpawnerType;

	/**
	* Spawn box pivot point
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner|Editor")
	ESpawnerPivotType PivotType;

	/**
	* Use if pivot point is not Centered
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner|Editor")
	FVector SpawnerSize;
	
	/**
	* The scale should be updated immediately upon change
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner|Editor")
	bool bInstantUpdate;	
	
	/**
	* Manually apply the scale
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawner|Editor")
	bool bApplyScale;

#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
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
