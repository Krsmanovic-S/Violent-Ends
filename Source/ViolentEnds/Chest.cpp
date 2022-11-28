#include "Chest.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseItem.h"
#include "BaseQuest.h"


AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = false;

	this->ChestCollision = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	RootComponent = this->ChestCollision;

	this->ChestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Chest Mesh"));
	this->ChestMesh->SetupAttachment(this->ChestCollision);

	this->InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(FName("Interaction Widget"));
	this->InteractionWidget->SetupAttachment(this->ChestCollision);
}

void AChest::BeginPlay()
{
	Super::BeginPlay();
	
	this->InteractionWidget->SetVisibility(false);

	this->InitializeChest();
}

void AChest::InitializeChest()
{
	APlayerCharacter* PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	this->InitializeLoot(this->InsideLoot, this->ChestLootTable, this->MaximalAmountOfItems, PlayerReference->CurrentLevel);
}

void AChest::InteractPure()
{
	if(!this->bWasInteractedWith || this->bCanBeUsedAgain)
	{
		// Chests can be a part of an objective from a quest so we handle it here.
		if(this->RelevantQuest != NULL && this->RelevantQuest->Objectives[this->InteractiveObjectiveIndex].bIsActive)
		{
			// This is so the objective marker doesn't point to an already used chest.
			this->RelevantQuest->Objectives[this->InteractiveObjectiveIndex].ConnectedActors.RemoveSingle(this);

			this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->InteractiveObjectiveIndex]);
		}

		// This calls the blueprint event for interacting.
		IInteractiveObject::Execute_Interact(this);
	
		this->InteractionWidget->SetVisibility(false);

		this->bEnableWidgetSettings = false;
		this->bWasInteractedWith = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacted item cannot be used again."));
	}
}