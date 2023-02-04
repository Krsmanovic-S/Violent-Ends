#include "GA_Shoot.h"

#include "AbilitySystemComponent.h"
#include "ViolentEnds/Projectile/ProjectileBase.h"

UGA_Shoot::UGA_Shoot()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Ability.Attack.Default")));
}

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	auto SpawnedProjectile =
		GetWorld()->SpawnActorDeferred<AProjectileBase>(AProjectileBase::StaticClass(), FTransform());

	FGameplayEffectSpecHandle GEHandle = ASC->MakeOutgoingSpec(HitGameplayEffect, 0, FGameplayEffectContextHandle());
	if (SpawnedProjectile)
	{
		//SpawnedProjectile->OwnerAbilitySystemComponent = MakeWeakObjectPtr<UAbilitySystemComponent>(ASC);
		SpawnedProjectile->OwnerAbilitySystemComponent = (ASC);
		SpawnedProjectile->HitGameplayEffect = GEHandle;

		SpawnedProjectile->FinishSpawning(FTransform());
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
