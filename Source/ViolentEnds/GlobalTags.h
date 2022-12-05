#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

struct VIOLENTENDS_API FGlobalTags : public FGameplayTagNativeAdder
{
	FGameplayTag Status_Invincible;

	static const FGlobalTags& Get();

protected:
	virtual void AddTags() override;

private:
	static FGlobalTags Tags;
};