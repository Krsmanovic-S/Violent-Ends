#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "QuestComponent.generated.h"

class UVE_ASC;
enum class EQuestObjectiveType: uint8;

UCLASS()
class VIOLENTENDS_API UQuestComponent : public UActorComponent {

	GENERATED_BODY()

public:

	/**
	* Reference to the player's ability system component (use the tag system to hold info about quests)
	*/
	TWeakObjectPtr<UVE_ASC> PlayerASC;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SpawnObjective(FTransform SpawnTransform, FGameplayTagContainer ObjectiveQuestTag, EQuestObjectiveType ObjectiveType);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool RegisterPlayer(UVE_ASC* Player);

	UFUNCTION()
	void QuestTagAppliedCallback(FGameplayTagContainer QuestTagApplied);
};