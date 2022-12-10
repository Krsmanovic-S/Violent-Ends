#include "BaseAbility.h"

#include "TimerManager.h"

bool UBaseAbility::CanCastAbility(UPARAM(ref) float& CurrentStamina, const bool bFreeCast)
{
	if (bFreeCast) { return true; }

	if (!this->bIsOnCooldown && CurrentStamina >= this->StaminaCostToCast)
	{
		CurrentStamina -= this->StaminaCostToCast;
		this->StartAbilityCooldown();

		return true;
	}

	return false;
}

void UBaseAbility::StartAbilityCooldown()
{
	this->bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownHandle, this, &UBaseAbility::ResetAbility, this->Cooldown, false);
}

void UBaseAbility::ResetAbility()
{
	this->bIsOnCooldown = false;
}
