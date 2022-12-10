#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

struct VIOLENTENDS_API FGlobalTags : public FGameplayTagNativeAdder
{
	FGameplayTag Status_Invincible;
	FGameplayTag Weapon_BouncyBullets;

	static const FGlobalTags& Get();

protected:
	virtual void AddTags() override;

private:
	static FGlobalTags Tags;
};