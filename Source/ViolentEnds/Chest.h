#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "InteractiveObject.h"
#include "LootSystem.h"

#include "Chest.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;
class UWidgetComponent;
class UDataTable;
class UBaseItem;

UCLASS()
class VIOLENTENDS_API AChest : public AActor, public IInteractiveObject, public ILootSystem
{
	GENERATED_BODY()

public:
	AChest();

	virtual void InteractPure() override;

protected:
	virtual void BeginPlay() override;

private:
	void InitializeChest();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ChestCollision;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ChestMesh;

	/* Small widget showing the interact input key
	   above the chest, used to visualy indicate
	   when the player can interact with it */
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InteractionWidget;

	/* Table to use when picking out loot, only
	   items that are here are valid to be picked */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Loot", meta = (AllowPrivateAccess = true))
	UDataTable* ChestLootTable;

	/* Highest amount of itmes that this chest can hold,
	   used as the maximum value when randomizing an integer
	   (range would be 0, this value) */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Loot", meta = (AllowPrivateAccess = true))
	int32 MaximalAmountOfItems = 1;

	/* Loot that will spawn upon opening the chest, generated by the
	   ILootSystem interface from which this chest inherits */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UBaseItem*> InsideLoot;
};
