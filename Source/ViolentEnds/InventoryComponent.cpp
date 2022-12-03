#include "InventoryComponent.h"
#include "BaseItem.h"
#include "EntityStats.h"
#include "PlayerCharacter.h"
#include "BaseGun.h"
#include "BaseWeapon.h"
#include "BaseAmmo.h"
#include "BaseQuest.h"
#include "Kismet/GameplayStatics.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	this->InventoryCurrentCapacity = 0;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	this->InventoryMaxCapacity = 27;
	this->CurrentItems.Init(NULL, this->InventoryMaxCapacity);

	for(auto& StartingItem : DefaultItems)
	{
		if(StartingItem)
		{
			AddItem(StartingItem);
		}
	}

	// Ammo stacks should be at 0 when inventory just starts.
	// This will be changed when save/load system is implemented.
	UBaseAmmo* CurrentAmmo;
	for(auto& Ammo : this->AmmoInventory)
	{
		CurrentAmmo = Cast<UBaseAmmo>(Ammo.GetDefaultObject());

		// Start with 100 of each ammo, just for debugging purposes.
		CurrentAmmo->ItemCurrentStack = 100;

		CurrentAmmo->OwningInventory = this;
	}

	// Base slots = 20
	// Weapon - index 20
	// Secondary - index 21
	// Helmet - index 22
	// Body - index 23
	// Legs - index 24
	// Boots - index 25
	// Arms - index 26
	this->WeaponSlotIndex = this->BasicSlotAmount;
	this->SecondaryWeaponSlotIndex = this->WeaponSlotIndex + 1;
	this->HelmetSlotIndex = this->SecondaryWeaponSlotIndex + 1;
	this->BodyArmorSlotIndex = this->HelmetSlotIndex + 1;
	this->LegsSlotIndex = this->BodyArmorSlotIndex + 1;
	this->BootsSlotIndex = this->LegsSlotIndex + 1;
	this->ArmsSlotIndex = this->BootsSlotIndex + 1;

	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::AddItem(UBaseItem* Item)
{
	if(!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item NULL."));

		return false;
	}

	if(Item->IsA<UBaseAmmo>())
	{
		return this->AddAmmo(Item);
	}

	if(InventoryCurrentCapacity == InventoryMaxCapacity &&
	   CurrentItems[InventoryMaxCapacity - 8]->ItemCurrentStack ==
	   CurrentItems[InventoryMaxCapacity - 8]->ItemMaxStack	)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full."));
		return false;		
	}
	
	// Increasing an items stack if the same type of item is 
	// already there, this can only be done in the base slots.
	// Equipment cannot be stacked anyways so just pass on it.
	if(!Item->bIsEquipment)
	{
		for(int i = 0; i < this->BasicSlotAmount; i++)
		{
			if(this->CurrentItems[i] != NULL &&
			this->CurrentItems[i]->GetClass() == Item->GetClass() &&
			this->CurrentItems[i]->ItemCurrentStack < this->CurrentItems[i]->ItemMaxStack)
			{
				this->CurrentItems[i]->ItemCurrentStack++;

				if(this->bItemRelevantToObjective)
				{
					this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->RelevantObjectiveIndex], true);
				}

				OnInventoryUpdated.Broadcast();
				return true;
			}		
		}
	}

	auto CopyItem = DuplicateObject(Item, NULL);
	CopyItem->OwningInventory = this;
	CopyItem->World = GetWorld();
	CopyItem->bWasInitialized = true;
	CopyItem->ItemCurrentStack = 1;
	CopyItem->ItemSlotIndex = -1;
	CopyItem->InitializeTooltipMap();

	// If we can auto equip something we should just
	// do it here and immidiatelly return true.
	if(this->AutoEquipItem(CopyItem))
	{
		if(this->bItemRelevantToObjective)
		{
			this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->RelevantObjectiveIndex], true);
		}
		return true;
	}

	// Adds a new item into the inventory.
	for(int i = 0; i < this->BasicSlotAmount; i++)
	{
		if(this->CurrentItems[i] == NULL)
		{
			this->CurrentItems[i] = CopyItem;
			this->CurrentItems[i]->ItemSlotIndex = i;
			this->InventoryCurrentCapacity++;

			if(this->bItemRelevantToObjective)
			{
				this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->RelevantObjectiveIndex], true);
			}

			OnInventoryUpdated.Broadcast();
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::AddAmmo(UBaseItem* InputAmmo)
{
	UBaseAmmo* Input = Cast<UBaseAmmo>(InputAmmo);
	UBaseAmmo* Ammo;

	switch(Input->AmmoFireStyle)
	{
		case EFiringStyle::Standard:
			Ammo = Cast<UBaseAmmo>(this->AmmoInventory[0]->GetDefaultObject());
			if(Ammo->ItemCurrentStack < Ammo->ItemMaxStack)
			{
				Ammo->ItemCurrentStack++;

				if(this->PlayerReference->Gun != NULL)
				{
					this->PlayerReference->Gun->UpdateAmmo();
				}

				return true;
			}
			
			break;
		case EFiringStyle::Burst:
			Ammo = Cast<UBaseAmmo>(this->AmmoInventory[1]->GetDefaultObject());
			if(Ammo->ItemCurrentStack < Ammo->ItemMaxStack)
			{
				Ammo->ItemCurrentStack++;

				if(this->PlayerReference->Gun != NULL)
				{
					this->PlayerReference->Gun->UpdateAmmo();
				}

				return true;
			}
			break;
		case EFiringStyle::Shotgun:
			Ammo = Cast<UBaseAmmo>(this->AmmoInventory[2]->GetDefaultObject());
			if(Ammo->ItemCurrentStack < Ammo->ItemMaxStack)
			{
				Ammo->ItemCurrentStack++;

				if(this->PlayerReference->Gun != NULL)
				{
					this->PlayerReference->Gun->UpdateAmmo();
				}

				return true;
			}
			break;
		case EFiringStyle::Sniper:
			Ammo = Cast<UBaseAmmo>(this->AmmoInventory[3]->GetDefaultObject());
			if(Ammo->ItemCurrentStack < Ammo->ItemMaxStack)
			{
				Ammo->ItemCurrentStack++;

				if(this->PlayerReference->Gun != NULL)
				{
					this->PlayerReference->Gun->UpdateAmmo();
				}

				return true;
			}
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Ammo type wasn't recognized."));
			break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Input ammo is at max stack, cannot add more."));

	return false;
}

int32 UInventoryComponent::GetAmmoStackSize(int32 ArrayIndex) const
{
	// This is used by the AmmoDisplay widget to get  
	// the stack size of each individual ammo.
	if(ArrayIndex > this->AmmoInventory.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to access AmmoInventory out of bounds."));

		return 0;
	}

	UBaseAmmo* Ammo = Cast<UBaseAmmo>(this->AmmoInventory[ArrayIndex]->GetDefaultObject());

	return Ammo->ItemCurrentStack;
}

bool UInventoryComponent::AutoEquipItem(UBaseItem* Item)
{
	switch(Item->Type)
	{
		case EItemType::Weapon:
			if(!this->CurrentItems[this->WeaponSlotIndex])
			{
				return this->MoveItemToEmptySlot(Item, this->WeaponSlotIndex);
			}
			break;	
		case EItemType::Helmet:
			if(!this->CurrentItems[this->HelmetSlotIndex])
			{
				return this->MoveItemToEmptySlot(Item, this->HelmetSlotIndex);
			}
			break;
		case EItemType::BodyArmor:
			if(!this->CurrentItems[this->BodyArmorSlotIndex])
			{
				return this->MoveItemToEmptySlot(Item, this->BodyArmorSlotIndex);
			}
			break;
		case EItemType::Legs:
			if(!this->CurrentItems[this->LegsSlotIndex])
			{
				return this->MoveItemToEmptySlot(Item, this->LegsSlotIndex);
			}	
			break;
		case EItemType::Boots:
			if(!this->CurrentItems[this->BootsSlotIndex])
			{
				return this->MoveItemToEmptySlot(Item, this->BootsSlotIndex);
			}	
			break;
		case EItemType::Arms:
			if(!this->CurrentItems[this->ArmsSlotIndex])
			{
				return this->MoveItemToEmptySlot(Item, this->ArmsSlotIndex);
			}	
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Item Type Not Recognized."));
	}

	return false;
}

bool UInventoryComponent::RemoveItem(const int32& ItemIndex, bool DropItem)
{
	this->IsItemRelevantToAnObjective(this->CurrentItems[ItemIndex]);

	// Function for removing items from the inventory, DropItem bool
	// determines do we want to entire drop the entire item stack.
	if(this->CurrentItems[ItemIndex])
	{
		if(this->CurrentItems[ItemIndex]->ItemCurrentStack == 1 || DropItem)
		{
			// Handle dropping of equipped items.
			if(ItemIndex >= this->BasicSlotAmount)
			{
				this->ApplyEquippedItemStats(this->CurrentItems[ItemIndex], true);
			}

			// Unequip the weapon if we dropped it.
			if(ItemIndex == this->WeaponSlotIndex)
			{
				this->InteractionWithWeaponSlot(ItemIndex, 0);
			}

			// Dropping the entire item will decrease objective progress
			// more than once. Depends on the items current stack.
			if(this->bItemRelevantToObjective)
			{
				for(int i = 0; i < this->CurrentItems[ItemIndex]->ItemCurrentStack; i++)
				{
					this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->RelevantObjectiveIndex], false);
				}
			}

			this->CurrentItems[ItemIndex]->OwningInventory = nullptr;
			this->CurrentItems[ItemIndex]->World = nullptr;
			this->CurrentItems[ItemIndex] = NULL;
			this->InventoryCurrentCapacity--;
		}
		else
		{
			this->CurrentItems[ItemIndex]->ItemCurrentStack -= 1;

			if(this->bItemRelevantToObjective)
			{
				this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->RelevantObjectiveIndex], false);
			}
		}

		OnInventoryUpdated.Broadcast();
		return true;
	}

	return false;
}

bool UInventoryComponent::MoveItemToEmptySlot(UBaseItem* Item, const int32& IndexToMoveTo)
{
	// Function for when we take an item and drop it to 
	// another slot in the inventory that is empty.
	if(Item == NULL || IndexToMoveTo > this->InventoryMaxCapacity)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveItemToEmptySlot Function Error."));
		return false;
	}

	int32 FromWhereIndex = Item->ItemSlotIndex;

	this->CurrentItems[IndexToMoveTo] = Item;

	// Slot index can be -1 if we are using the auto equip 
	// method, that is why we need this check here.
	if(FromWhereIndex != -1)
	{
		this->CurrentItems[Item->ItemSlotIndex]->OwningInventory = nullptr;
		this->CurrentItems[Item->ItemSlotIndex]->World = nullptr;
		this->CurrentItems[Item->ItemSlotIndex] = NULL;
	}

	// We interacted with the Weapon slot.
	if(FromWhereIndex == this->WeaponSlotIndex || IndexToMoveTo == this->WeaponSlotIndex)
	{
		this->InteractionWithWeaponSlot(FromWhereIndex, IndexToMoveTo);
	}

	// Handle whenever we equip or de-equip an item in a slot.
	if(IndexToMoveTo >= this->BasicSlotAmount)
	{
		this->ApplyEquippedItemStats(Item);
	}
	else if(FromWhereIndex >= this->BasicSlotAmount)
	{
		this->ApplyEquippedItemStats(Item, true);
	}

	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::SwapItems(UBaseItem* Item, const int32& ToWhereIndex)
{
	// Function for when we take an item and drop it onto another
	// and we want them to swap places in the inventory.
	if(!this->CurrentItems[Item->ItemSlotIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("First item doesn't exist."));
		return false;
	}

	int32 FromWhereIndex = Item->ItemSlotIndex;

	this->CurrentItems[Item->ItemSlotIndex] = this->CurrentItems[ToWhereIndex];
	this->CurrentItems[ToWhereIndex] = Item;

	if(ToWhereIndex == this->WeaponSlotIndex)
	{
		this->InteractionWithWeaponSlot(FromWhereIndex, ToWhereIndex, true);
	}

	// Handle the swapping of equipped items.
	if(FromWhereIndex >= this->BasicSlotAmount)
	{
		this->ApplyEquippedItemStats(this->CurrentItems[FromWhereIndex]);

		this->ApplyEquippedItemStats(this->CurrentItems[ToWhereIndex], true);
	}

	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::MergeItems(const int32& WhereToMerge, class UBaseItem* Item)
{
	// Function for when we drop an item onto a same type item.
	if(!this->CurrentItems[WhereToMerge] || Item == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("MergeItems Function Error."));
		return false;		
	}

	int32 FromWhereMerge = Item->ItemSlotIndex;

	while(this->CurrentItems[WhereToMerge]->ItemCurrentStack < this->CurrentItems[WhereToMerge]->ItemMaxStack 
	      && Item->ItemCurrentStack != 0)
	{
		this->CurrentItems[WhereToMerge]->ItemCurrentStack++;
		Item->ItemCurrentStack--;
	}

	if(Item->ItemCurrentStack == 0)
	{
		this->RemoveItem(Item->ItemSlotIndex, true);
	}
	
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::SplitItemStack(UBaseItem* Item, const int32& SlotToSplitTo, int32 AmountToSplit)
{
	// Function for spliting an item stack from one slot
	// to another, uses an UMG to set the amount to split.
	if(Item == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("SplitItemStack Function Error."));
		return false;
	}

	int32 StackAmount = Item->ItemCurrentStack;

	int32 FromWhereSplit = Item->ItemSlotIndex;

	auto CopyItem = DuplicateObject(Item, NULL);

	CopyItem->ItemSlotIndex = SlotToSplitTo;

	this->CurrentItems[SlotToSplitTo] = CopyItem;

	if(AmountToSplit == 0)
	{
		if(StackAmount % 2 != 0)
		{
			Item->ItemCurrentStack = (StackAmount + 1) / 2;

			this->CurrentItems[SlotToSplitTo]->ItemCurrentStack = (StackAmount - 1) / 2;
		}
		else
		{
			Item->ItemCurrentStack = StackAmount / 2;

			this->CurrentItems[SlotToSplitTo]->ItemCurrentStack = StackAmount / 2;
		}
	}
	else
	{
		Item->ItemCurrentStack = StackAmount - AmountToSplit;

		this->CurrentItems[SlotToSplitTo]->ItemCurrentStack = AmountToSplit;
	}

	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::UnequipItem(UBaseItem* Item)
{
	for(int i = 0; i < this->BasicSlotAmount; i++)
	{
		if(this->CurrentItems[i] == NULL)
		{
			return this->MoveItemToEmptySlot(Item, i);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No free slots available."));

	return false;
}

void UInventoryComponent::ApplyEquippedItemStats(UBaseItem* Item, bool RemoveStats)
{
	// Handles applying/de-applying stats in case of equipping/de-equipping.
	auto& Entity = this->PlayerReference->PlayerStats;

	if(Item != NULL)
	{
		if(!RemoveStats)
		{
			Entity->MaximumHealth += Item->ItemStats.ItemAddHealth;
			Entity->HealthRegenerationAmount += Item->ItemStats.ItemHealthRegen;

			Entity->MaximumStamina += Item->ItemStats.ItemAddStamina;
			Entity->StaminaRegenerationAmount += Item->ItemStats.ItemStaminaRegen;
		
			Entity->ArmorValue += Item->ItemStats.ItemArmor;
			Entity->FireResistance += Item->ItemStats.ItemFireResist;

			Entity->GlobalDamageBonus += Item->ItemStats.ItemDamage;
			Entity->FireDamageBonus += Item->ItemStats.ItemFireDamage;
			Entity->CriticalHitChance += Item->ItemStats.ItemCriticalChance;
			Entity->CriticalHitDamageMultiplier += Item->ItemStats.ItemCriticalDamage;
		}
		else
		{
			Entity->MaximumHealth = FMath::Max(Entity->MaximumHealth - Item->ItemStats.ItemAddHealth, 0);
			Entity->HealthRegenerationAmount = FMath::Max(Entity->HealthRegenerationAmount - Item->ItemStats.ItemHealthRegen, 0);
			
			Entity->ArmorValue = FMath::Max(Entity->ArmorValue - Item->ItemStats.ItemArmor, 0);
			Entity->FireResistance = FMath::Max(Entity->FireResistance - Item->ItemStats.ItemFireResist, 0);
			
			Entity->MaximumStamina = FMath::Max(Entity->MaximumStamina - Item->ItemStats.ItemAddStamina, 0);
			Entity->StaminaRegenerationAmount = FMath::Max(Entity->StaminaRegenerationAmount - Item->ItemStats.ItemStaminaRegen, 0);

			Entity->GlobalDamageBonus = FMath::Max(Entity->GlobalDamageBonus - Item->ItemStats.ItemDamage, 0);
			Entity->FireDamageBonus = FMath::Max(Entity->FireDamageBonus - Item->ItemStats.ItemFireDamage, 0);
			Entity->CriticalHitChance = FMath::Max(Entity->CriticalHitChance - Item->ItemStats.ItemCriticalChance, 0);
			Entity->CriticalHitDamageMultiplier = FMath::Max(Entity->CriticalHitDamageMultiplier - Item->ItemStats.ItemCriticalDamage, 0);
		}

		this->OnStatsUpdated.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL Item passed in ApplyEquippedItemStats()."));
	}
}

void UInventoryComponent::InteractionWithWeaponSlot(const int32& FromWhereIndex, const int32& ToWhereIndex, const bool bSwapping)
{
	// We took a gun and unequiped it.
	if(FromWhereIndex == this->WeaponSlotIndex || bSwapping)
	{
		this->PlayerReference->UnequipWeapon();
	}

	// We equipped a gun.
	if(ToWhereIndex == this->WeaponSlotIndex)
	{
		this->PlayerReference->EquipWeapon();
	}
}

void UInventoryComponent::IsItemRelevantToAnObjective(const UBaseItem* ItemToCheck)
{
	if(this->RelevantQuest != NULL)
	{
		UBaseItem* ObjectiveItem;

		for(int32 ObjectiveIndex : this->CollectionObjectiveIndexes)
		{
			ObjectiveItem = Cast<UBaseItem>(this->RelevantQuest->Objectives[ObjectiveIndex].ItemToCollect->GetDefaultObject());

			if(ItemToCheck->ItemDisplayName.ToString().Equals(ObjectiveItem->ItemDisplayName.ToString()))
			{
				this->bItemRelevantToObjective = true;

				this->RelevantObjectiveIndex = ObjectiveIndex;

				return;
			}
		}
	}
	this->bItemRelevantToObjective = false;
}
