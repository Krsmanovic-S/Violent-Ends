#pragma once

#include "CoreMinimal.h"

#include "CharacterInventoryComponent.generated.h"

class UItemBase;
class UEquipmentBase;
class UWeaponBase;
class UVE_ASC;
enum class EEquipmentType : uint8;

static int32 InventoryWidth = 6;
static int32 InventoryHeight = 6;

USTRUCT(BlueprintType)
struct VIOLENTENDS_API FInventoryItem
{
	GENERATED_BODY()

public:
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Iventory|Item")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAddedToInventory, UEquipmentBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryRefreshed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipped, UEquipmentBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemoved, UEquipmentBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemUsed, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSecondaryUsed, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponSlotted, UWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponSwapped, UWeaponBase*, From, UWeaponBase*, To);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponSheathed, UWeaponBase*, Weapon);

UCLASS()
class VIOLENTENDS_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterInventoryComponent();

	// Array of items in the inventory
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TArray<UItemBase*> Inventory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UVE_ASC* OwnersAbilitySystemComponent;

	// Item interaction event
	//
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FInventoryRefreshed OnInventoryRefeshed;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemAddedToInventory OnItemAddedToInventory;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemEquipped OnItemEquipped;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemRemoved OnItemRemoved;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemUsed OnItemUsed;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemSecondaryUsed OnItemSecondaryUsed;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FWeaponSlotted OnWeaponUsed;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FWeaponSwapped OnWeaponSwapped;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FWeaponSheathed OnWeaponSheathed;

	// Equipped weapon inventory
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UWeaponBase* Weapon1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UWeaponBase* Weapon2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UWeaponBase* Weapon3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UWeaponBase* Weapon4;

	// Equipped gear inventory
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UEquipmentBase* Helmet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UEquipmentBase* BodyArmor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UEquipmentBase* Legs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UEquipmentBase* Boots;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UEquipmentBase* ArmsGuard;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	void InitInventoryComponent(UVE_ASC* OwnerASC);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	int32 FindItemIndexInInventory(UItemBase* ItemToFind);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	int32 FindFirstEmptySlot(UItemBase* ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool TryAddItemToInventory(UItemBase* ItemToAdd, int32& Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool TryEquipItem(UEquipmentBase* ItemToEquip);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	UEquipmentBase* TryRemoveGear(EEquipmentType EquipmentTypeToRemove);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool TryRemoveWeapon(int32 WeaponSlotNumber);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool TryGetWeaponOfSlot(int32 WeaponSlotNumber, UEquipmentBase*& OutWeapon);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool TryGetEquippedGear(EEquipmentType EquipmentTypeToRemove, UEquipmentBase*& OutGear);

	/**
	* Attempt to assign weapon to a slot
	* @param NewWeapon The new weapon to equip
	* @param Slot The index of the slot to equip to
	* @return True if weapon equip is successful
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool AttachWeaponToSlot(UWeaponBase* NewWeapon, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool UseWeaponAtSlot(int32 Slot);

	/**
	* Attempt to get the current weapon
	* @param[out] OutCurrentWeapon The current weapon object
	* @return True if current weapon object is not null
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	bool TryGetCurrentWeapon(UWeaponBase* OutCurrentWeapon);
	/**
	 *Remove current weapon from hands(similar to sheathing a sword)
	 * @return Index of weapon (1, 2, 3, 4)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Functionality")
	int32 SheatheWeapon();

protected:

	UEquipmentBase** GetGearSlotRef(EEquipmentType GearType);
	UWeaponBase** GetWeaponSlotRef(int32 SlotIndex);

};
