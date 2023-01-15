#include "EquipmentLegs.h"

UEquipmentLegs::UEquipmentLegs() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.Legs"));
}