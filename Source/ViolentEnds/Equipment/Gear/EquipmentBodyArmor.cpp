#include "EquipmentBodyArmor.h"

UEquipmentBodyArmor::UEquipmentBodyArmor() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.BodyArmor"));
	EquipmentTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.BodyArmor"));
}