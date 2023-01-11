#include "GE_EquipmentEffect.h"

UGE_EquipmentEffect::UGE_EquipmentEffect()
{
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Equipment.Equip.Effect")));
}

UGE_EquipmentEffect* UGE_EquipmentEffect::CreateEffect(FEquipmentEffectSpec EffectSpec)
{
	// Create new effect
	UGE_EquipmentEffect* NewEffect = NewObject<UGE_EquipmentEffect>();

	// Create tag for the effect
	FName EffectTag;
	switch (EffectSpec.EquipmentType)
	{
		case EEquipmentType::Weapon:
			EffectTag = TEXT("Character.Equipment.Equip.Weapon");
			break;
		case EEquipmentType::Helmet:
			EffectTag = TEXT("Character.Equipment.Equip.Helmet");
			break;
		case EEquipmentType::BodyArmor:
			EffectTag = TEXT("Character.Equipment.Equip.BodyArmor");
			break;
		case EEquipmentType::Legs:
			EffectTag = TEXT("Character.Equipment.Equip.Legs");
			break;
		case EEquipmentType::Boots:
			EffectTag = TEXT("Character.Equipment.Equip.Boots");
			break;
		case EEquipmentType::Arms:
			EffectTag = TEXT("Character.Equipment.Equip.Arms");
			break;
	}

	// Add the effect tag
	NewEffect->InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(EffectTag));

	// Add the modifiers
	auto& EffectSpecList = EffectSpec.Stats;
	for (auto& Item : EffectSpecList) {
		FGameplayModifierInfo ModInfo;
		ModInfo.Attribute = Item.AttributeAffected;
		ModInfo.Magnitude = Item.Magnitude;

		NewEffect->Modifiers.Add(ModInfo);
	}

	return NewEffect;
}
