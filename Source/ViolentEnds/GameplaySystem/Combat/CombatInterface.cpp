#include "CombatInterface.h"

bool ICombatInterface::TryUseAbilityWithTag(const FGameplayTagContainer& AbilityTag)
{
	return false;
}

bool ICombatInterface::TryUseDefaultAttack()
{
	return false;
}
