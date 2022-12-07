#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


// Delegate that Blueprints will use to update the UI.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatsUpdate);

class UBaseItem;
class UBaseAmmo;
class APlayerCharacter;
class UBaseQuest;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIOLENTENDS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	

	// -----------------------------------------------------------------------------------------------
	// Functions
	UFUNCTION(BlueprintCallable)
	bool AddItem(UBaseItem* Item);

	UFUNCTION(BlueprintCallable)
	bool AddAmmo(UBaseItem* InputAmmo);

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoStackSize(int32 ArrayIndex) const;

	UFUNCTION(BlueprintCallable)
	bool AutoEquipItem(UBaseItem* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(const int32& ItemIndex, bool DropItem = false);

	UFUNCTION(BlueprintCallable)
	bool MoveItemToEmptySlot(UBaseItem* Item, const int32& IndexToMoveTo);

	UFUNCTION(BlueprintCallable)
	bool SwapItems(UBaseItem* Item, const int32& ToWhereIndex);

	UFUNCTION(BlueprintCallable)
	bool MergeItems(const int32& WhereToMerge, UBaseItem* Item);

	UFUNCTION(BlueprintCallable)
	bool SplitItemStack(UBaseItem* Item, const int32& SlotToSplitTo, int32 AmountToSplit = 0);

	UFUNCTION(BlueprintCallable)
	bool UnequipItem(UBaseItem* Item);

	UFUNCTION(BlueprintCallable)
	void ApplyEquippedItemStats(UBaseItem* Item, bool RemoveStats = false);

	UFUNCTION(BlueprintCallable)
	void InteractionWithWeaponSlot(const int32& FromWhereIndex, const int32& ToWhereIndex, const bool bSwapping = false);

	UFUNCTION(BlueprintCallable)
	void IsItemRelevantToAnObjective(const UBaseItem* ItemToCheck);
	// -----------------------------------------------------------------------------------------------

	int32 InventoryCurrentCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 InventoryMaxCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<TSubclassOf<UBaseAmmo>> AmmoInventory;

	UPROPERTY(BlueprintReadWrite)
	TArray<UBaseItem*> DefaultItems;

	UPROPERTY(BlueprintReadWrite)
	TArray<UBaseItem*> CurrentItems;

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter* PlayerReference;

	UBaseQuest* RelevantQuest;

	TArray<int32> CollectionObjectiveIndexes;

	UPROPERTY(BlueprintReadWrite)
	bool bItemRelevantToObjective;

	UPROPERTY(BlueprintReadWrite)
	int32 RelevantObjectiveIndex;

	UPROPERTY(BlueprintAssignable)
	FInventoryUpdate OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)
	FStatsUpdate OnStatsUpdated;

	int32 BasicSlotAmount = 20;
	int32 WeaponSlotIndex;
	int32 SecondaryWeaponSlotIndex;
	int32 HelmetSlotIndex;
	int32 BodyArmorSlotIndex;
	int32 LegsSlotIndex;
	int32 BootsSlotIndex;
	int32 ArmsSlotIndex;
};
