#include "EquipmentArms.h"

UEquipmentArms::UEquipmentArms() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Gear.Arms"));
}