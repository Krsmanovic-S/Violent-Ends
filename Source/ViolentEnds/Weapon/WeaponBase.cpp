#include "WeaponBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"

UWeaponBase::UWeaponBase()
{
	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon"));
	EquipmentTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Effect.Item.Equipment.Weapon"));
}

bool UWeaponBase::TryUseItem_Implementation(AActor* Target)
{
	bool Success = Super::TryUseItem_Implementation(Target);

	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target))
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			auto Context = ASC->MakeEffectContext();
			Context.AddSourceObject(this);
			auto Handle = ASC->MakeOutgoingSpec(WeaponAbilityEffect, 1.0f, Context);
			if (Handle.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Amount of abilities granted: %d"), Handle.Data.Get()->GrantedAbilitySpecs.Num())
				for (auto Item : Handle.Data.Get()->GrantedAbilitySpecs)
				{
					UE_LOG(LogTemp, Display, TEXT("Granting ability: %s"),
						*Item.Ability.GetDefaultObject()->GetFName().ToString())
				}
					ASC->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
			}

			return true;
		}
	}

	return Success;
}

bool UWeaponBase::TryUseItemSecondary_Implementation(AActor* Target)
{
	return false;
}

bool UWeaponBase::CanBeUsed_Implementation(AActor* Target)
{
	return true;
}

void UWeaponBase::ShootEffect()
{
	Reserve -= AmmoPerShot;
}

void UWeaponBase::ReloadEffect(int32 AmmoReloaded)
{
	Reserve = FMath::Min(AmmoReloaded, MagazineSize);
}

bool UWeaponBase::IsOutOfAmmo()
{
	int32 NewAmmo = Reserve - AmmoPerShot;
	return NewAmmo <= 0;
}

bool UWeaponBase::HasEnoughAmmoToShoot()
{
	return Reserve >= AmmoPerShot;
}
