#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "VE_ASC.generated.h"

UCLASS()
class VIOLENTENDS_API UVE_ASC: public UAbilitySystemComponent {

	GENERATED_BODY()
public:

	UVE_ASC();

	UFUNCTION(BlueprintCallable, Category = "Ability|Effect")
	FGameplayEffectSpecHandle GetEffectWithTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Ability|Effect")
	bool TryRemoveEffectWithTag(FGameplayTag Tag);
		
	UFUNCTION(BlueprintCallable, Category = "Ability|Effect")
	bool HasEffectedByTag(FGameplayTag Tag);

	UFUNCTION()
	void EffectAppliedToSelfCallback(UAbilitySystemComponent* Taregt, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
};