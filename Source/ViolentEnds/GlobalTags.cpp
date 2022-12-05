#include "ViolentEnds/GlobalTags.h"

#include "GameplayTagsManager.h"

FGlobalTags FGlobalTags::Tags;

const FGlobalTags& FGlobalTags::Get()
{
	return Tags;
}

void FGlobalTags::AddTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	Manager.AddNativeGameplayTag(TEXT("Status.Invincible"));
}