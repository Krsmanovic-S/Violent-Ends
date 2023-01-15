#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "QuestInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestInterface : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API IQuestInterface
{
	GENERATED_BODY()

public:

	virtual bool IsPartOfQuest(FGameplayTagContainer QuestTag) const = 0;

	virtual FGameplayTagContainer GetQuest() const = 0;

	
};
