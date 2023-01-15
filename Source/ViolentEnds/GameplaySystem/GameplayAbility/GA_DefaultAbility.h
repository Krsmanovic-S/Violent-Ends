#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"

#include "GA_DefaultAbility.generated.h"

UCLASS()
class VIOLENTENDS_API UGA_DefaultAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_DefaultAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};