#include "EquipmentBodyArmor.h"

UEquipmentBodyArmor::UEquipmentBodyArmor() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Gear.BodyArmor"));
}