#include "EquipmentBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ViolentEnds/GameplaySystem/GameplayEffect/EquipmentEffect/GE_EquipmentEffect.h"

UEquipmentBase::UEquipmentBase()
{
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment"));
}

bool UEquipmentBase::TryUseItem_Implementation(AActor* Target)
{
	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target))
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			// If the equipment doesn't have the correct tag then the effect tag cannot be created and will not be
			// removed so we don't apply it
			FGameplayTag EffectTagMaster = GetEffectTagFromEquipmentTag(ItemTag);
			if (!EffectTagMaster.IsValid()) { return false; }
			FString TagString = EffectTagMaster.ToString();
			TagString.Append(TEXT("Rarity"));

			// Make the effect for rarity
			FEquipmentEffectSpec EffectSpec;
			EffectSpec.EquipmentType = EquipmentType;
			EffectSpec.Stats = StatList;

			// Create dynamic effect for rarity
			UGE_EquipmentEffect* EquipmentEffect = UGE_EquipmentEffect::CreateEffect(EffectSpec);
			EquipmentEffect->InheritableGameplayEffectTags.Added.AddTag(ItemTag);
			FGameplayEffectSpec EquipmentEffectSpec;
			EquipmentEffectSpec.Def = EquipmentEffect;
			EquipmentEffectSpec.SetLevel(0);

			// Apply the effect for rarity
			ASC->ApplyGameplayEffectSpecToSelf(EquipmentEffectSpec);

			// Apply the effect for base
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EquipmentBaseStats, 0, EffectContext);

			if (EffectSpecHandle.IsValid()) { 
				FActiveGameplayEffectHandle GAHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); 
			}

			return true;
		}
	}

	return false;
}

bool UEquipmentBase::TryUseItemSecondary_Implementation(AActor* Target)
{
	return false;
}

bool UEquipmentBase::CanBeUsed_Implementation(AActor* Target)
{
	return true;
}

FGameplayTag UEquipmentBase::GetEffectTagFromEquipmentTag(FGameplayTag EquipmentTag)
{
	FGameplayTag RetVal = FGameplayTag();

	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Gear.Arms"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Arms"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Gear.BodyArmor"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.BodyArmor"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Gear.Boots"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Boots"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Gear.Helmet"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Helmet"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Gear.Legs"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Gear.Legs"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Weapon.Melee"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon.Melee"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Weapon.Pistol"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon.Pistol"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Weapon.Rifle"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon.Rifle"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Weapon.Shotgun"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon.Shotgun"));
	}
	if (EquipmentTag == FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment.Weapon.Sniper"), false))
	{
		RetVal = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon.Sniper"));
	}

	return RetVal;
}
