#include "EquipmentBase.h"
#include "AbilitySystemInterface.h"
#include "ViolentEnds/GameplaySystem/GameplayEffect/EquipmentEffect/GE_EquipmentEffect.h"
#include "AbilitySystemComponent.h"

UEquipmentBase::UEquipmentBase() {
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment"));


}

bool UEquipmentBase::TryUseItem_Implementation(AActor* Target)
{
	if(IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target)) { 
		
		if (UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent()) {

			FEquipmentEffectSpec EffectSpec;
			EffectSpec.EquipmentType = EquipmentType;
			EffectSpec.Stats = StatList;

			UGE_EquipmentEffect* EquipmentEffect = UGE_EquipmentEffect::CreateEffect(EffectSpec);
			FGameplayEffectSpec EquipmentEffectSpec;
			EquipmentEffectSpec.Def = EquipmentEffect;
			EquipmentEffectSpec
				.SetLevel(0);

			ASC->ApplyGameplayEffectSpecToSelf(EquipmentEffectSpec);

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
