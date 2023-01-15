#pragma once

#include "CoreMinimal.h"

#include "GameplayEffect.h"

#include "QuestEffectBase.generated.h"

UCLASS()
class VIOLENTENDS_API UQuestEffectBase : public UGameplayEffect
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	UFUNCTION()
	void SetQuestEffectTag(FGameplayTagContainer TagContainer);
};