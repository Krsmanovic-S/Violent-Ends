#include "BaseItem.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "PhysicalDamageType.h"
#include "FireDamageType.h"


UBaseItem::UBaseItem()
{
    this->ItemDisplayName = FText::FromString("Item");
    this->ItemCurrentStack = 1;
    this->ItemMaxStack = 1;
}

void UBaseItem::InitializeTooltipMap()
{
    float PhysicalDamage = 0;
    float FireDamage = 0;

    if(this->ItemStats.ItemDamageTypes.Find(UPhysicalDamageType::StaticClass()))
    {
        PhysicalDamage = this->ItemStats.ItemDamageTypes[UPhysicalDamageType::StaticClass()];
    }

    if(this->ItemStats.ItemDamageTypes.Find(UFireDamageType::StaticClass()))
    {
        FireDamage = this->ItemStats.ItemDamageTypes[UFireDamageType::StaticClass()];
    }

    this->MapForTooltips = {
        {"Physical Damage", PhysicalDamage},
        {"Fire Damage", FireDamage},
        {"Damage", this->ItemStats.ItemDamage},
        {"Fire Damage", this->ItemStats.ItemFireDamage},
        {"Critical Chance", this->ItemStats.ItemCriticalChance},
        {"Critical Damage", this->ItemStats.ItemCriticalDamage},
        {"Maximum Health", this->ItemStats.ItemAddHealth},
        {"HP Regen", this->ItemStats.ItemHealthRegen},
        {"Armor", this->ItemStats.ItemArmor},
        {"Fire Resistance", this->ItemStats.ItemFireResist},
        {"Maximum Stamina", this->ItemStats.ItemAddStamina},
        {"Stamina Regen", this->ItemStats.ItemStaminaRegen},
        {"Dash Cost", this->ItemStats.ItemDashCost}
    };
}

void UBaseItem::Use(APlayerCharacter* Player)
{
    int32 SlotIndex;

    switch(this->Type)
    {
        case EItemType::Helmet:
            SlotIndex = Player->PlayerInventory->HelmetSlotIndex;
            break;
        case EItemType::BodyArmor:
            SlotIndex = Player->PlayerInventory->BodyArmorSlotIndex;
            break;
        case EItemType::Legs:
            SlotIndex = Player->PlayerInventory->LegsSlotIndex;
            break;
        case EItemType::Boots:
            SlotIndex = Player->PlayerInventory->BootsSlotIndex;
            break;
        case EItemType::Arms:
            SlotIndex = Player->PlayerInventory->ArmsSlotIndex;
            break;
        default:
            SlotIndex = -1;
    }

    // Equip or swap depending on is something in that slot already.
    if(SlotIndex != -1)
    {
        if(Player->PlayerInventory->CurrentItems[SlotIndex] != NULL)
        {
            // If we called this method on an item already in the appropriate
            // equipment slot, then we should just unequip it and return early.
            if(Player->PlayerInventory->CurrentItems[SlotIndex] == this)
            {
                Player->PlayerInventory->UnequipItem(this);

                return;
            }

            Player->PlayerInventory->SwapItems(Player->PlayerInventory->CurrentItems[SlotIndex], this->ItemSlotIndex);
        }
        else
        {
            Player->PlayerInventory->MoveItemToEmptySlot(this, SlotIndex);
        }
    }
}