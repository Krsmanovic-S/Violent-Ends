#include "BaseAbility.h"
#include "Engine/World.h"
#include "TimerManager.h"


bool UBaseAbility::CanCastAbility(const float& CurrentStamina , const bool bFreeCast)
{
    if(bFreeCast)
    {
        return true;
    }

    if(CurrentStamina >= this->StaminaCostToCast && !this->bIsOnCooldown)
    {
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
