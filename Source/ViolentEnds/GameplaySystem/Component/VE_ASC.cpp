#include "VE_ASC.h"

UVE_ASC::UVE_ASC() {

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UVE_ASC::EffectAppliedToSelfCallback);
}

FGameplayEffectSpecHandle UVE_ASC::GetEffectWithTag(FGameplayTag Tag)
{
	return FGameplayEffectSpecHandle();
}

bool UVE_ASC::TryRemoveEffectWithTag(FGameplayTag Tag)
{
	bool Success = HasEffectedByTag(Tag);
	if (!Success) return false;

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	// It should only return 1 effect
	auto Effects = GetActiveEffectsWithAllTags(TagContainer);
	
	if (Effects.Num() < 1) return false;

	FActiveGameplayEffectHandle EffectHandle = Effects[0];

	RemoveActiveGameplayEffect(EffectHandle);

	return true;
}

bool UVE_ASC::HasEffectedByTag(FGameplayTag Tag)
{
	bool Result = HasMatchingGameplayTag(Tag);
	return Result;
}

void UVE_ASC::EffectAppliedToSelfCallback(
	UAbilitySystemComponent* Taregt, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{

}
