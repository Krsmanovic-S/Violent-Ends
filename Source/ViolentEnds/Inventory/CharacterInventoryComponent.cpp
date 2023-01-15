#include "CharacterInventoryComponent.h"

#include "ViolentEnds/Equipment/EquipmentBase.h"
#include "ViolentEnds/Equipment/EquipmentType.h"
#include "ViolentEnds/GameplaySystem/Component/VE_ASC.h"

static TMap<EEquipmentType, FGameplayTag> GearTagMap = {
	{ EEquipmentType::Weapon, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon")) },
	{ EEquipmentType::Helmet, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Helmet")) },
	{ EEquipmentType::BodyArmor, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.BodyArmor")) },
	{ EEquipmentType::Legs, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Legs")) },
	{ EEquipmentType::Boots, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Boots")) },
	{ EEquipmentType::Arms, FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Arms")) },
};

UCharacterInventoryComponent::UCharacterInventoryComponent()
{

}

void UCharacterInventoryComponent::InitInventoryComponent(UVE_ASC* OwnerASC) {
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
	if (FoundIndex >= 0) {
	
		Inventory[FoundIndex] = ItemToAdd;
		Index = FoundIndex;
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
		// TODO: Add functionality
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

	return EquippedItem;
}

bool UCharacterInventoryComponent::TryRemoveWeapon(int32 WeaponSlotNumber)
{
	return false;
}

bool UCharacterInventoryComponent::TryGetWeaponOfSlot(int32 WeaponSlotNumber, UEquipmentBase*& OutWeapon)
{
	return false;
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
