#include "QuestComponent.h"
#include "ViolentEnds/GameplaySystem/Component/VE_ASC.h"
#include "ViolentEnds/Quest/Objective/QuestObjectiveTypes.h"
#include "ViolentEnds/Quest/Objective/QuestObjective.h"


void UQuestComponent::SpawnObjective(
	FTransform SpawnTransform, FGameplayTagContainer ObjectiveQuestTag, EQuestObjectiveType ObjectiveType)
{
	if (ObjectiveType == EQuestObjectiveType::Destination) { 
		auto QuestObjective = GetWorld()->SpawnActorDeferred<AQuestObjectiveBase>(AQuestObjectiveBase::StaticClass(),SpawnTransform);
		if (QuestObjective) { 
			
			QuestObjective->FinishSpawning(SpawnTransform);
		}
	}

}

bool UQuestComponent::RegisterPlayer(UVE_ASC* Player)
{
	if (!Player) return false;

	PlayerASC = MakeWeakObjectPtr<UVE_ASC>(Player);

	bool Success = PlayerASC.IsValid();
	if (!Success) return false;

	Player->OnQuestTagAdded.AddDynamic(this, &UQuestComponent::QuestTagAppliedCallback);

	return true;
}

void UQuestComponent::QuestTagAppliedCallback(FGameplayTagContainer QuestTagApplied) {

}
