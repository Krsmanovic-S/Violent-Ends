#include "VE_ASC.h"

UVE_ASC::UVE_ASC()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UVE_ASC::EffectAppliedToSelfCallback);
	OnGameplayEffectAppliedDelegateToTarget.AddUObject(this, &UVE_ASC::EffectAppliedToTargetCallback);

	AbilityFailedCallbacks.AddLambda([&](const UGameplayAbility* Ability, const FGameplayTagContainer& TagContainer) {
		UE_LOG(LogTemp, Warning, TEXT("%s failed to activate, reason: %s"), *Ability->GetFName().ToString(),
			*TagContainer.ToString())
	});
	AbilityActivatedCallbacks.AddLambda([&](const UGameplayAbility* Ability) {
		UE_LOG(LogTemp, Warning, TEXT("%s with tag(s) %s activated"), *Ability->GetFName().ToString())
	});

	OnAnyGameplayEffectRemovedDelegate().AddLambda([&](const FActiveGameplayEffect& EffectRemoved) {
		ABILITY_LOG(Warning, TEXT("%s removed"), *EffectRemoved.Spec.ToSimpleString())
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

	UE_LOG(LogTemp, Warning, TEXT("Effect appied: %s"), *EffectSpec.ToSimpleString())

	auto& Added = EffectSpec.Def->InheritableOwnedTagsContainer.Added;
	auto& Removed = EffectSpec.Def->InheritableOwnedTagsContainer.Removed;

	for (int32 i = 0; i < Added.Num(); ++i)
	{
		ABILITY_LOG(Display, TEXT("Tag added: %s"), *Added.GetByIndex(i).ToString())
	}
	for (int32 i = 0; i < Removed.Num(); ++i)
	{
		ABILITY_LOG(Display, TEXT("Tag added: %s"), *Removed.GetByIndex(i).ToString())
	}

	TArray<FString> TextOut;
	for (int32 i = 0; i < EffectSpec.ModifiedAttributes.Num(); i++)
	{
		TextOut.Add(EffectSpec.ModifiedAttributes[i].Attribute.AttributeName + TEXT(" with magnitude of : ")
					+ FString::SanitizeFloat(EffectSpec.ModifiedAttributes[i].TotalMagnitude));
	}

	for (int32 i = 0; i < TextOut.Num(); i++)
	{
		ABILITY_LOG(Warning, TEXT("%s"), *TextOut[i])
	}
}

void UVE_ASC::EffectAppliedToTargetCallback(
	UAbilitySystemComponent* Target, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.Def->GetOwnedGameplayTags(TagContainer);

	// TODO: Add actual tag(s) here
	bool Success = TagContainer.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Quest")));
	if (Success)
	{
		if (OnQuestTagAdded.IsBound()) { OnQuestTagAdded.Broadcast(TagContainer); }
	}
}

int32 UVE_ASC::HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT(""))))
	{
		if (OnCharacterHit.IsBound()) { OnCharacterHit.Broadcast(EventTag); }
	}

	return int32();
}
