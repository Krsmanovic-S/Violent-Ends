#pragma once

#include "CoreMinimal.h"

#include "GA_DefaultAbility.h"

#include "GA_Shoot.generated.h"

class UProjectileBase;

UCLASS()
class VIOLENTENDS_API UGA_Shoot : public UGA_DefaultAbility
{
	GENERATED_BODY()

public:

	UGA_Shoot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};