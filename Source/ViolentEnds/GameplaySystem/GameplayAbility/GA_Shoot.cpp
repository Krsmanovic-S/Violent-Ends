#include "GA_Shoot.h"
#include "ViolentEnds/Projectile/ProjectileBase.h"
#include "AbilitySystemComponent.h"

UGA_Shoot::UGA_Shoot() {
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Ability.Attack.Default")));
}

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(AProjectileBase::StaticClass(), FTransform());
	if (SpawnedProjectile) {
		SpawnedProjectile->OwnerAbilitySystemComponent =
			MakeWeakObjectPtr<UAbilitySystemComponent>(ASC);
		SpawnedProjectile->FinishSpawning(FTransform());
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}