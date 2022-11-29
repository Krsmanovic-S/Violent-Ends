#include "BaseAbility.h"
#include "TimerManager.h"


bool UBaseAbility::CanCastAbility(float& CurrentStamina , const bool bFreeCast)
{
    if(bFreeCast)
    {
        return true;
    }

    if(CurrentStamina >= this->StaminaCostToCast && !this->bIsOnCooldown)
    {
        CurrentStamina -= this->StaminaCostToCast;
        return true;
    }

    return false;
}

void UBaseAbility::StartAbilityCooldown()
{
    GetWorld()->GetTimerManager().SetTimer(CooldownHandle, this, &UBaseAbility::ResetAbility, 
									            this->Cooldown, false);
}

void UBaseAbility::ResetAbility()
{
    this->bIsOnCooldown = false;
}
