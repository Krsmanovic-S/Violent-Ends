#include "EquipmentArms.h"

UEquipmentArms::UEquipmentArms() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.Arms"));
	EquipmentTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.Arms"));

}