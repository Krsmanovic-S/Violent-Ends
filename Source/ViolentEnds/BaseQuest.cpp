#include "BaseQuest.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "BaseItem.h"
#include "ObjectiveLocation.h"
#include "InteractiveObject.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"


void UBaseQuest::SetUpObjectives()
{
    bool bCanAssign = true;
    bool bHasActiveObjective = false;

    for(auto& Objective : this->Objectives)
    {
        if(Objective.bIsCompleted)
        {
            Objective.bIsActive = false;

            Objective.bIsFocused = false;

            continue;          
        }

        if(bCanAssign)
        {
            if(!bHasActiveObjective)
            {
                Objective.bIsActive = true;

                Objective.bIsFocused = true;

                bHasActiveObjective = true;
            }
            else if(!Objective.bIsChainable)
            {
                Objective.bIsActive = true;
            }
            else
            {
                bCanAssign = false;
            }
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
    for(uint8 i = 0; i < this->Objectives.Num(); i++)
    {
        if(this->Objectives[i].Type == EObjectiveType::Collect)
        {
            Player->PlayerInventory->RelevantQuest = this;

            Player->PlayerInventory->CollectionObjectiveIndexes.Add(i);

            continue;
        }
        else
        {
            if(this->Objectives[i].Type == EObjectiveType::Location || this->Objectives[i].Type == EObjectiveType::Interact)
            {
                UGameplayStatics::GetAllActorsOfClassWithTag(this->GetWorld(), this->Objectives[i].ActorClassToLookFor, this->Objectives[i].TagToLookFor, this->Objectives[i].ConnectedActors);
            }
            // For Defeat-type objectives.
            else
            {
                UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), this->Objectives[i].ActorClassToLookFor, this->Objectives[i].ConnectedActors);
            }

            for(auto& Actor : this->Objectives[i].ConnectedActors)
            {
                switch(this->Objectives[i].Type)
                {
                    case EObjectiveType::Location:
                        LocationActor = Cast<AObjectiveLocation>(Actor);

                        if(LocationActor != NULL)
                        {
                            LocationActor->RelevantQuest = this;
                            LocationActor->LocationObjectiveIndex = i;
                        }
                        break;
                    case EObjectiveType::Interact:
                        InteractionActor = Cast<IInteractiveObject>(Actor);

                        if(InteractionActor != NULL)
                        {
                            InteractionActor->RelevantQuest = this;
                            InteractionActor->InteractiveObjectiveIndex = i;
                        }
                        break;
                    case EObjectiveType::Defeat:
                        EnemyActor = Cast<ABaseEnemy>(Actor);

                        if(EnemyActor != NULL)
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
}

void UBaseQuest::UpdateObjective(FQuestObjective& Objective, bool bIncreaseProgress)
{
    if(bIncreaseProgress && Objective.CurrentProgress != Objective.FinalProgress)
    {
        Objective.CurrentProgress++;

        if(Objective.CurrentProgress == Objective.FinalProgress)
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
	if(Player->CurrentActiveQuest->QuestType == EQuestType::Completed)
	{
		for(auto* Quest : Player->AllQuests)
		{
			if(Quest->QuestType != EQuestType::Completed)
			{
				Player->CurrentActiveQuest = Quest;

                break;
			}
		}
	}

    Player->OnQuestUpdated.Broadcast();
}

void UBaseQuest::SelectNextQuest(APlayerCharacter* Player, UBaseQuest* FollowUpQuest)
{
    UBaseQuest* Prerequisite;

    int32 PrerequisiteIndex;

    for(auto& QuestClass : FollowUpQuest->PrerequisiteQuests)
    {
        PrerequisiteIndex = -1;

        // Don't check for this quest, we already know it exists and is completed.
        if(QuestClass != this->GetClass())
        {
            Prerequisite = Cast<UBaseQuest>(QuestClass->GetDefaultObject());

            // Try to find the prerequisite quest in the AllQuests array.
            for(int32 i = 0; i < Player->AllQuests.Num(); i++)
            {
                if(Player->AllQuests[i]->GetClass() == QuestClass)
                {
                    PrerequisiteIndex = i;

                    break;
                }
            }

            // If the Prerequisite doesn't exist we need to add it
            // and then just update the active quest for the Player.
            if(PrerequisiteIndex == -1)
            {
                Player->AllQuests.Add(Prerequisite);

                return this->UpdatePlayerQuest(Player);
            }
            // If the prerequisite exists but isn't completed, that is
            // our next active quest. Signal to the UI and start the new quest.
            else if(Player->AllQuests[PrerequisiteIndex]->QuestType != EQuestType::Completed)
            {
                Player->CurrentActiveQuest = Player->AllQuests[PrerequisiteIndex];

                Player->OnQuestUpdated.Broadcast();

                return;
            }
        }   
    }

    // Everything above going through means that the only left
    // possibility is to just continue to our desired next quest.
    Player->AllQuests.Add(FollowUpQuest);
    Player->CurrentActiveQuest = Player->AllQuests[Player->AllQuests.Num() - 1];

    Player->OnQuestUpdated.Broadcast();
}

void UBaseQuest::OnQuestCompleted(APlayerCharacter* Player)
{
    if(this->Reward.ExperienceReward > 0)
    {
        Player->AddXP(this->Reward.ExperienceReward);
    }

    // Handles adding of the reward items to the Player.
    // Will drop the item if it cannot be added (i.e. full inventory).
    UBaseItem* ItemToAdd;
    for(auto& MapProperty : this->Reward.ItemReward)
    {
        ItemToAdd = Cast<UBaseItem>(MapProperty.Key->GetDefaultObject());
        
        for(int i = 1; i <= MapProperty.Value; i++)
        {
            // If we cannot add the reward item (i.e. inventory full),
            // we should just drop the item into the world.
            if(Player->PlayerInventory->AddItem(ItemToAdd) == false)
            {
                UBaseItem* CopyItem = DuplicateObject(ItemToAdd, NULL);

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

    if(this->NextQuest != NULL)
    {
        UBaseQuest* FollowUpQuest = Cast<UBaseQuest>(this->NextQuest->GetDefaultObject());

        // Only this quest was a prerequisite for the next one.
        // Means we can just go to it immidiatelly.
        if(FollowUpQuest->PrerequisiteQuests.Num() <= 1)
        {
            Player->AllQuests.Add(FollowUpQuest);

            Player->CurrentActiveQuest = Player->AllQuests[Player->AllQuests.Num() - 1];

            Player->OnQuestUpdated.Broadcast();
        }
        else
        {
            this->SelectNextQuest(Player, FollowUpQuest);
        }
    }
}