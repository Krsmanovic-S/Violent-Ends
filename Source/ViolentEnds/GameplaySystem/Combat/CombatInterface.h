#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API ICombatInterface
{
	GENERATED_BODY()

public:

	/**
	* Attempt to use the ability with the specified tag
	* @return True if the ability was used successfully
	*/
	virtual bool TryUseAbilityWithTag(FGameplayTag AbilityTag) const = 0;

	/**
	* Attempt to use the default attack ability
	* @return True if the ability was used successfully
	*/
	virtual bool TryUseDefaultAttack() const = 0;

};
