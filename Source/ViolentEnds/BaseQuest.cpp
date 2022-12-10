#include "BaseQuest.h"

#include "BaseEnemy.h"
#include "BaseItem.h"
#include "Engine/World.h"
#include "InteractiveObject.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectiveLocation.h"
#include "PlayerCharacter.h"
#include "ViolentEnds/LogMacros.h"

void UBaseQuest::SetUpObjectives()
{
	bool bCanAssign = true;
	bool bHasActiveObjective = false;

	for (auto& Objective : this->Objectives)
	{
		if (Objective.bIsCompleted)
		{
			Objective.bIsActive = false;
			Objective.bIsFocused = false;

			continue;
		}

		if (bCanAssign)
		{
			if (!bHasActiveObjective)
			{
				Objective.bIsActive = true;
				Objective.bIsFocused = true;
				bHasActiveObjective = true;
			}
			else if (!Objective.bIsChainable) { Objective.bIsActive = true; }
			else { bCanAssign = false; }
		}
	}
}

void UBaseQuest::OnQuestBegin()
{
	AObjectiveLocation* LocationActor;
	IInteractiveObject* InteractionActor;
	ABaseEnemy* EnemyActor;

	this->World = GEngine->GameViewport->GetWorld();

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// Setting up objectives for the UI.
	for (uint8 i = 0; i < this->Objectives.Num(); i++)
	{
		if (this->Objectives[i].Type == EObjectiveType::Collect)
		{
			Player->PlayerInventory->RelevantQuest = this;
			Player->PlayerInventory->CollectionObjectiveIndexes.Add(i);

			continue;
		}
		if (this->Objectives[i].Type == EObjectiveType::Location
			|| this->Objectives[i].Type == EObjectiveType::Interact)
		{
			UGameplayStatics::GetAllActorsOfClassWithTag(this->GetWorld(), this->Objectives[i].ActorClassToLookFor,
				this->Objectives[i].TagToLookFor, this->Objectives[i].ConnectedActors);
		}
		// For Defeat-type objectives.
		else
		{
			UGameplayStatics::GetAllActorsOfClass(
				this->GetWorld(), this->Objectives[i].ActorClassToLookFor, this->Objectives[i].ConnectedActors);
		}

		for (auto& Actor : this->Objectives[i].ConnectedActors)
		{
			switch (this->Objectives[i].Type)
			{
				case EObjectiveType::Location:
					LocationActor = Cast<AObjectiveLocation>(Actor);

					if (LocationActor != nullptr)
					{
						LocationActor->RelevantQuest = this;
						LocationActor->LocationObjectiveIndex = i;
					}
					break;
				case EObjectiveType::Interact:
					InteractionActor = Cast<IInteractiveObject>(Actor);

					if (InteractionActor != nullptr)
					{
						InteractionActor->RelevantQuest = this;
						InteractionActor->InteractiveObjectiveIndex = i;
					}
					break;
				case EObjectiveType::Defeat:
					EnemyActor = Cast<ABaseEnemy>(Actor);

					if (EnemyActor != nullptr)
					{
						EnemyActor->RelevantQuest = this;
						EnemyActor->DefeatObjectiveIndex = i;
					}
					break;
				default:
					UE_LOG(LogTemp, Warning, TEXT("Objective type not recognized."));
					return;
			}
		}
	}
}

void UBaseQuest::UpdateObjective(FQuestObjective& Objective, bool bIncreaseProgress)
{
	if (bIncreaseProgress && Objective.CurrentProgress != Objective.FinalProgress)
	{
		Objective.CurrentProgress++;

		if (Objective.CurrentProgress == Objective.FinalProgress)
		{
			Objective.bIsCompleted = true;
			Objective.bIsFocused = false;
		}
	}
	else
	{
		Objective.CurrentProgress--;
		Objective.bIsCompleted = false;
	}

	this->RefreshObjectives.Broadcast();
}

void UBaseQuest::UpdatePlayerQuest(APlayerCharacter* Player)
{
	if (Player->CurrentActiveQuest->IsCompleted())
	{
		for (auto* Quest : Player->AllQuests)
		{
			if (!Quest->IsCompleted())
			{
				Player->SetActiveQuest(Quest);

				break;
			}
		}
	}
}

void UBaseQuest::SelectNextQuest(APlayerCharacter* Player, TSubclassOf<UBaseQuest> FollowUpQuest)
{
	for (TSubclassOf<UBaseQuest> PrerequisiteQuest : FollowUpQuest->GetDefaultObject<UBaseQuest>()->PrerequisiteQuests)
	{
		UBaseQuest* PlayerQuest = Player->FindQuestByClass(PrerequisiteQuest);

		// If a pre-requisite quest does not exist, need to give it to the player and make it the active one
		if (PlayerQuest == nullptr)
		{
			LOG(LogTemp, "Granting quest");
			Player->AddQuest(FollowUpQuest, true);
			return;
		}

		// If the pre-requisite quest does exist but is not completed, mark it as active
		if (!PlayerQuest->IsCompleted())
		{
			LOG(LogTemp, "Updating active quest");
			Player->SetActiveQuest(PlayerQuest);
			return;
		}
	}

	// If all pre-requisites are taken care of, grant the followup quest
	LOG(LogTemp, "Granting followup");
	Player->AddQuest(FollowUpQuest, true);
}

void UBaseQuest::OnQuestCompleted(APlayerCharacter* Player)
{
	if (this->Reward.ExperienceReward > 0) { Player->AddXP(this->Reward.ExperienceReward); }

	// Handles adding of the reward items to the Player.
	// Will drop the item if it cannot be added (i.e. full inventory).
	UBaseItem* ItemToAdd;
	for (auto& MapProperty : this->Reward.ItemReward)
	{
		ItemToAdd = Cast<UBaseItem>(MapProperty.Key->GetDefaultObject());

		for (int i = 1; i <= MapProperty.Value; i++)
		{
			// If we cannot add the reward item (i.e. inventory full),
			// we should just drop the item into the world.
			if (Player->PlayerInventory->AddItem(ItemToAdd) == false)
			{
				UBaseItem* CopyItem = DuplicateObject(ItemToAdd, nullptr);
				CopyItem->ItemCurrentStack = MapProperty.Value - i + 1;
				// BP event for dropping, 2nd parameter tells if
				// the item came from the inventory or not.
				Player->DropItem(CopyItem, false);

				break;
			}
		}
	}

	// Don't forget to clear this array, the quest might have
	// had some collection objectives which would fill it up.
	Player->PlayerInventory->CollectionObjectiveIndexes.Empty();
	this->QuestType = EQuestType::Completed;

	if (this->NextQuest == nullptr) { return; }

	UBaseQuest* FollowupQuest = this->NextQuest->GetDefaultObject<UBaseQuest>();
	// Only this quest was a prerequisite for the next one.
	// Means we can just go to it immediately.
	if (FollowupQuest->PrerequisiteQuests.Num() <= 1) { Player->AddQuest(this->NextQuest, true); }
	else { this->SelectNextQuest(Player, this->NextQuest); }
}

bool UBaseQuest::IsCompleted()
{
	return QuestType == EQuestType::Completed;
}