#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "VE_WorldSubsystem.generated.h"

class UGameplayEffect;
class UVE_ASC;

/**
 * The subsystem that represent the world's ability to interact with actors via the ability system
 */
UCLASS()
class VIOLENTENDS_API UVE_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UVE_WorldSubsystem();

	/**
	 * Attempt to apply the effect class to the target
	 * @param EffectToApply the UClass of the gameplay effect to apply
	 * @param TargetActor the target actor to be applied the gameplay effect to
	 * @return True if effect is applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "World|Interaction")
	bool TryApplyEffectToTargetActor(TSubclassOf<UGameplayEffect> EffectToApply, AActor* TargetActor);

	/**
	 * Attempt to apply the effect class to the target
	 * This version acccept a context handle
	 * @param EffectToApply the UClass of the gameplay effect to apply
	 * @param TargetActor the target actor to be applied the gameplay effect to
	 * @param ContextHandle the context of the gameplay effect to be used
	 * @return True if effect is applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "World|Interaction")
	bool TryApplyEffectToTargetActorWithContext(
		TSubclassOf<UGameplayEffect> EffectToApply, AActor* TargetActor, FGameplayEffectContextHandle& ContextHandle);

	/**
	 * Attempt to apply the effect class to the target
	 * @param EffectToApply the pointer to the dynamically created gameplay effect to apply to the target
	 * @param TargetActor the target actor to be applied the gameplay effect to
	 * @return True if effect is applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "World|Interaction")
	bool TryApplyDynamicEffectToTargetActor(UGameplayEffect* EffectToApply, AActor* TargetActor);

	/**
	 * Attempt to apply the effect class to the target
	 * This version accept a context handle
	 * @param EffectToApply the pointer to the dynamically created gameplay effect to apply to the target
	 * @param TargetActor the target actor to be applied the gameplay effect to
	 * @param ContextHandle the context of the gameplay effect to be used
	 * @return True if effect is applied successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "World|Interaction")
	bool TryApplyDynamicEffectToTargetActorWithContext(
		UGameplayEffect* EffectToApply, AActor* TargetActor, FGameplayEffectContextHandle& ContextHandle);

	/**
	 * Return the world's ability system component
	 * Returns the base ASC type, if you need to use custom class you can cast it afterwards
	 */
	UFUNCTION(BlueprintCallable, Category = "World|Interaction")
	UAbilitySystemComponent* GetWorldASC();

	/**
	 * Attempt to get the ability system component from the target actor
	 * @param TargetActor the actor to try to get the ASC of
	 * @param OutTargetAbilitySystemComponent [OUT] the pointer to the ASC of the actor if success
	 * @return True if success
	 */
	UFUNCTION(BlueprintCallable, Category = "World|Interaction")
	bool TryGetAbilitySystemComponentFromActor(
		AActor* TargetActor, UAbilitySystemComponent*& OutTargetAbilitySystemComponent);

protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	UVE_ASC* WorldAbilitySystemComponent;
};