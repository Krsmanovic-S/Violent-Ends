#include "CharacterInventoryComponent.h"

#include "ViolentEnds/Equipment/EquipmentBase.h"
#include "ViolentEnds/Equipment/EquipmentType.h"
#include "ViolentEnds/GameplaySystem/Component/VE_ASC.h"
#include "ViolentEnds/Weapon/WeaponBase.h"

static TMap<EEquipmentType, FGameplayTag> GearTagMap = {
	{ EEquipmentType::Weapon, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon")) },
	{ EEquipmentType::Helmet, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Helmet")) },
	{ EEquipmentType::BodyArmor,
		FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.BodyArmor")) },
	{ EEquipmentType::Legs, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Legs")) },
	{ EEquipmentType::Boots, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Boots")) },
	{ EEquipmentType::Arms, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Arms")) },
};

UCharacterInventoryComponent::UCharacterInventoryComponent() {}

void UCharacterInventoryComponent::InitInventoryComponent(UVE_ASC* OwnerASC)
{
	OwnersAbilitySystemComponent = OwnerASC;
}

int32 UCharacterInventoryComponent::FindItemIndexInInventory(UItemBase* ItemToFind)
{
	return Inventory.Find(ItemToFind);
}

int32 UCharacterInventoryComponent::FindFirstEmptySlot(UItemBase* ItemToAdd)
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (!Inventory[i]) { return i; }
	}

	return INDEX_NONE;
}

bool UCharacterInventoryComponent::TryAddItemToInventory(UItemBase* ItemToAdd, int32& Index)
{
	int32 FoundIndex = FindFirstEmptySlot(ItemToAdd);
	if (FoundIndex >= 0)
	{
		Inventory[FoundIndex] = ItemToAdd;
		Index = FoundIndex;
		if (OnInventoryRefeshed.IsBound()) { OnInventoryRefeshed.Broadcast(); }
		return true;
	}
	return false;
}

bool UCharacterInventoryComponent::TryEquipItem(UEquipmentBase* ItemToEquip)
{
	if (!OwnersAbilitySystemComponent) { return false; }
	EEquipmentType EquipmentType = ItemToEquip->EquipmentType;

	if (EquipmentType == EEquipmentType::Weapon)
	{
		// Find first empty slot
		int32 EmptyIndex = INDEX_NONE;
		if (!Weapon1) { EmptyIndex = 1; }
		else if (!Weapon2) { EmptyIndex = 2; }
		else if (!Weapon3) { EmptyIndex = 3; }
		else if (!Weapon4) { EmptyIndex = 4; }

		// There's an empty slot
		if (EmptyIndex != INDEX_NONE)
		{
			auto NewWeapon = CastChecked<UWeaponBase>(ItemToEquip);
			AttachWeaponToSlot(NewWeapon, EmptyIndex);
		}

		// There's no empty slot
		return false;
	}

	int32 Index = FindItemIndexInInventory(ItemToEquip);
	if (Index >= 0)
	{
		// If the item is in inventory, remove it
		Inventory[Index] = nullptr;
	}

	// Get the gear slot
	UEquipmentBase** GearSlot = GetGearSlotRef(EquipmentType);

	// Get the current equipped gear
	UEquipmentBase* CurrentEquippedGear = TryRemoveGear(EquipmentType);

	// Assign slotted gear to ItemToEquip
	*GearSlot = ItemToEquip;

	// Attempt to apply the effect to the owner
	ItemToEquip->TryUseItem(GetOwner());

	if (CurrentEquippedGear)
	{
		// If the item source is from inventory, swap them
		if (Index >= 0) { Inventory[Index] = CurrentEquippedGear; }
	}
	else
	{
		// Attempt to add the removed item to inventory
		int32 NewItemIndex;
		TryAddItemToInventory(CurrentEquippedGear, NewItemIndex);
	}
	if (OnInventoryRefeshed.IsBound()) { OnInventoryRefeshed.Broadcast(); }
	return true;
}

UEquipmentBase* UCharacterInventoryComponent::TryRemoveGear(EEquipmentType EquipmentTypeToRemove)
{
	if (!OwnersAbilitySystemComponent) { return nullptr; }

	// Get the gear slot
	UEquipmentBase** Equipped = GetGearSlotRef(EquipmentTypeToRemove);

	UEquipmentBase* EquippedItem = *Equipped;
	OwnersAbilitySystemComponent->TryRemoveEffectWithTag(GearTagMap[EquipmentTypeToRemove]);

	// Unequip the gear
	*Equipped = nullptr;
	if (OnInventoryRefeshed.IsBound()) { OnInventoryRefeshed.Broadcast(); }
	return EquippedItem;
}

bool UCharacterInventoryComponent::TryRemoveWeapon(int32 WeaponSlotNumber)
{
	UWeaponBase** SlotRef = GetWeaponSlotRef(WeaponSlotNumber);
	UWeaponBase* WeaponRef = nullptr;
	if (!SlotRef) { return false; }
	WeaponRef = *SlotRef;
	if (!WeaponRef) { return false; }
	int32 NewIndex;

	if (!TryAddItemToInventory(WeaponRef, NewIndex)) { return false; }
	WeaponRef = nullptr;
	if (OnInventoryRefeshed.IsBound()) { OnInventoryRefeshed.Broadcast(); }
	return true;
}

bool UCharacterInventoryComponent::TryGetWeaponOfSlot(int32 WeaponSlotNumber, UEquipmentBase*& OutWeapon)
{
	auto SlotRef = GetWeaponSlotRef(WeaponSlotNumber);
	UWeaponBase* WeaponRef = nullptr;

	if (!SlotRef) { return false; }
	WeaponRef = *SlotRef;
	if (!WeaponRef) { return false; }

	OutWeapon = WeaponRef;
	return true;
}

bool UCharacterInventoryComponent::TryGetEquippedGear(EEquipmentType EquipmentTypeToRemove, UEquipmentBase*& OutGear)
{
	switch (EquipmentTypeToRemove)
	{
		case EEquipmentType::Helmet:
			OutGear = Helmet;
			break;
		case EEquipmentType::BodyArmor:
			OutGear = BodyArmor;
			break;
		case EEquipmentType::Legs:
			OutGear = Legs;
			break;
		case EEquipmentType::Boots:
			OutGear = Boots;
			break;
		case EEquipmentType::Arms:
			OutGear = ArmsGuard;
			break;
		default:
			// Do not deal with weapon
			return false;
	}

	return (OutGear != nullptr);
}

bool UCharacterInventoryComponent::AttachWeaponToSlot(UWeaponBase* NewWeapon, int32 Slot)
{
	UWeaponBase** SlotRef = GetWeaponSlotRef(Slot);
	if (!SlotRef)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid slot"))
		return false; // Invalid slot
	}
	UItemBase* ItemInSlot = *SlotRef;

	int32 Index = FindItemIndexInInventory(NewWeapon);
	if (Index >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found in inventory at index %d"), Index)
		// If the item is in inventory, swap with current weapon slot it
		if (ItemInSlot)
		{
			UItemBase* Swap = ItemInSlot;
			*SlotRef = NewWeapon;
			Inventory[Index] = Swap;
		}
		else
		{
			// The item in weapon slot is empty, remove New Weapon from inventory and add to slot
			*SlotRef = NewWeapon;
			Inventory[Index] = nullptr;
		}
	}
	if (OnInventoryRefeshed.IsBound()) { OnInventoryRefeshed.Broadcast(); }
	return true;
}

bool UCharacterInventoryComponent::UseWeaponAtSlot(int32 Slot)
{
	UWeaponBase** SlotRef = GetWeaponSlotRef(Slot);
	
	// If invalid slot index, return try to equip failed
	if (!SlotRef) { return false; }
	// If slot is empty, return try to equip failed
	if (!(*SlotRef)) { return false; }

	// If already equipped said weapon
	if (CurrentWeapon == *SlotRef) return false;

	UWeaponBase* OldWeapon = CurrentWeapon;
	UWeaponBase* NewWeapon = *SlotRef;

	// Swap weapon
	CurrentWeapon = NewWeapon;

	OnWeaponSwapped.Broadcast(OldWeapon, NewWeapon);
	CurrentWeapon->TryUseItem(GetOwner());
	if (OnInventoryRefeshed.IsBound()) { OnInventoryRefeshed.Broadcast(); }
	return true;
}

bool UCharacterInventoryComponent::TryGetCurrentWeapon(UWeaponBase* OutCurrentWeapon)
{
	if (!CurrentWeapon) { return false; }

	OutCurrentWeapon = CurrentWeapon;
	return true;
}

int32 UCharacterInventoryComponent::SheatheWeapon()
{
	int32 Index = INDEX_NONE;
	if (CurrentWeapon)
	{
		OwnersAbilitySystemComponent->TryRemoveEffectWithTag(GearTagMap[EEquipmentType::Weapon]);
		OnWeaponSheathed.Broadcast(CurrentWeapon);

		if (CurrentWeapon == Weapon1) { Index = 1; }
		if (CurrentWeapon == Weapon2) { Index = 2; }
		if (CurrentWeapon == Weapon3) { Index = 3; }
		if (CurrentWeapon == Weapon4) { Index = 4; }

		CurrentWeapon = nullptr;
	}

	return Index;
}

UEquipmentBase** UCharacterInventoryComponent::GetGearSlotRef(EEquipmentType GearType)
{
	UEquipmentBase** GearSlot = &Helmet;
	switch (GearType)
	{
		case EEquipmentType::Helmet:
			GearSlot = &Helmet;
			break;
		case EEquipmentType::BodyArmor:
			GearSlot = &BodyArmor;
			break;
		case EEquipmentType::Legs:
			GearSlot = &Legs;
			break;
		case EEquipmentType::Boots:
			GearSlot = &Boots;
			break;
		case EEquipmentType::Arms:
			GearSlot = &ArmsGuard;
			break;
		default:
			return nullptr;
	}

	return GearSlot;
}

UWeaponBase** UCharacterInventoryComponent::GetWeaponSlotRef(int32 SlotIndex)
{
	UWeaponBase** SlotRef = nullptr;
	switch (SlotIndex)
	{
		case 0:
			SlotRef = &Weapon1;
			break;
		case 1:
			SlotRef = &Weapon2;
			break;
		case 2:
			SlotRef = &Weapon3;
			break;
		case 3:
			SlotRef = &Weapon4;
			break;

		default:
			break;
	}
	return SlotRef;
}
