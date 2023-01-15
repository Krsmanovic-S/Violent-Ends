#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	/**
	 * Attempt to use the ability with the specified tag
	 * @return True if the ability was used successfully
	 */
	virtual bool TryUseAbilityWithTag(const FGameplayTagContainer& AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	/**
	 * Attempt to use the default attack ability
	 * @return True if the ability was used successfully
	 */
	virtual bool TryUseDefaultAttack();
};
