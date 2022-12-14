#include "BaseAbility.h"

#include "PlayerCharacter.h"
#include "TimerManager.h"

void UBaseAbility::InitializeWorldFromPlayer(APawn* PlayerPawn)
{
	this->World = PlayerPawn->GetWorld();

	if (this->World == nullptr) { UE_LOG(LogTemp, Warning, TEXT("World not set for ability class.")); }
}

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

	if (this->World != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle, this, &UBaseAbility::ResetAbility, this->Cooldown, false);
	}
}

void UBaseAbility::ResetAbility()
{
	this->bIsOnCooldown = false;
}