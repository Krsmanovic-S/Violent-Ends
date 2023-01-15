#include "QuestEffectBase.h"

#if WITH_EDITOR
void UQuestEffectBase::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) {
	
}
#endif //WITH_EDITOR


void UQuestEffectBase::SetQuestEffectTag(FGameplayTagContainer TagContainer) {
	for (int32 i = 0; i < TagContainer.Num(); i++)
	{
		InheritableOwnedTagsContainer.AddTag(TagContainer.GetByIndex(i));
	}
}