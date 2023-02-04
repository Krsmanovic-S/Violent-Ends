#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"

#include "VE_ASC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestTagAdded, FGameplayTagContainer, TagContainer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHitWithTag, FGameplayTag, HitTags);

UCLASS()
class VIOLENTENDS_API UVE_ASC : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UVE_ASC();

	UPROPERTY(BlueprintAssignable, Category = "Ability|Effect")
	FQuestTagAdded OnQuestTagAdded;

	UPROPERTY(BlueprintAssignable, Category = "Ability|Effect")
	FCharacterHitWithTag OnCharacterHit;

	UFUNCTION(BlueprintCallable, Category = "Ability|Effect")
	FGameplayEffectSpecHandle GetEffectWithTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Ability|Effect")
	bool TryRemoveEffectWithTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Ability|Effect")
	bool HasEffectedByTag(FGameplayTag Tag);

	UFUNCTION()
	void EffectAppliedToSelfCallback(UAbilitySystemComponent* Source, const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION()
	void EffectAppliedToTargetCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle EffectHandle);

	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;
};