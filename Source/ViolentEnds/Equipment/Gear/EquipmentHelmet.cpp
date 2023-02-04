#include "EquipmentHelmet.h"

UEquipmentHelmet::UEquipmentHelmet() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.Helmet"));
	EquipmentTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Item.Equipment.Gear.Helmet"));
}