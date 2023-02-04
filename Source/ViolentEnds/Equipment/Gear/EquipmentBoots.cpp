#include "EquipmentBoots.h"

UEquipmentBoots::UEquipmentBoots() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.Boots"));
	EquipmentTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.Boots"));
}