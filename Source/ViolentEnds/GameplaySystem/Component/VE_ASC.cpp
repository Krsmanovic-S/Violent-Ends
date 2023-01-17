#include "VE_ASC.h"

UVE_ASC::UVE_ASC()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UVE_ASC::EffectAppliedToSelfCallback);
	AbilityFailedCallbacks.AddLambda([&](const UGameplayAbility* Ability, const FGameplayTagContainer& TagContainer) {
		UE_LOG(LogTemp, Warning, TEXT("%s failed to activate, reason: %s"), *Ability->GetFName().ToString(),
			*TagContainer.ToString())
	});
	AbilityActivatedCallbacks.AddLambda([&](const UGameplayAbility* Ability) {
		UE_LOG(LogTemp, Warning, TEXT("%s with tag(s) %s activated"), *Ability->GetFName().ToString())
	});
}

FGameplayEffectSpecHandle UVE_ASC::GetEffectWithTag(FGameplayTag Tag)
{
	return FGameplayEffectSpecHandle();
}

bool UVE_ASC::TryRemoveEffectWithTag(FGameplayTag Tag)
{
	bool Success = HasEffectedByTag(Tag);
	if (!Success) { return false; }

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(Tag);

	// It should only return 1 effect
	auto Effects = GetActiveEffectsWithAllTags(TagContainer);

	if (Effects.Num() < 1) { return false; }

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
	UAbilitySystemComponent* Source, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	UE_LOG(LogTemp, Warning, TEXT("Effect appied: %s"), *EffectSpec.ToSimpleString())
	// Check if the effect is quest based
	FGameplayTagContainer TagContainer;
	EffectSpec.Def->GetOwnedGameplayTags(TagContainer);

	// TODO: Add actual tag(s) here
	bool Success = TagContainer.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Quest")));
	if (Success)
	{
		if (OnQuestTagAdded.IsBound()) { OnQuestTagAdded.Broadcast(TagContainer); }
	}

	if (!Source)
	{
		UE_LOG(LogTemp, Error, TEXT("Target invalid"))
		return;
	}
}
