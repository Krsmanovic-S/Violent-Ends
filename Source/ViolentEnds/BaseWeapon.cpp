#include "BaseWeapon.h"

#include "BaseMeleeWeapon.h"
#include "InventoryComponent.h"
#include "PlayerCharacter.h"

void UBaseWeapon::Use(class APlayerCharacter* Player)
{
	int32 WeaponIndex = Player->PlayerInventory->WeaponSlotIndex;

	if (Player->PlayerInventory->CurrentItems[WeaponIndex] != NULL)
	{
		// Unequip the weapon if we use it while equipped.
		if (Player->PlayerInventory->CurrentItems[WeaponIndex] == this) { Player->PlayerInventory->UnequipItem(this); }
		else
		{
			auto* PreviousWeapon = Cast<UBaseWeapon>(Player->PlayerInventory->CurrentItems[WeaponIndex]);

			Player->PlayerInventory->SwapItems(this, PreviousWeapon->ItemSlotIndex);
		}
	}
	else { Player->PlayerInventory->MoveItemToEmptySlot(this, WeaponIndex); }
}