#include "BaseEnemy.h"
#include "BaseGun.h"
#include "EntityStats.h"
#include "BaseItem.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BaseQuest.h"
#include "EnemyAIController.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	this->EnemyStats = CreateDefaultSubobject<UEntityStats>(TEXT("Enemy Stats"));

	this->PerceptionStimuliComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimulus"));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(this->BlueprintGunClass != NULL)
	{
		FActorSpawnParameters SpawnParameters;

		SpawnParameters.Owner = this;

		this->RangedWeapon = GetWorld()->SpawnActor<ABaseGun>(this->BlueprintGunClass, SpawnParameters);
		this->RangedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunAttachPoint"));
	}

	this->bCanAttack = true;

	APlayerCharacter* PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	this->InitializeLoot(this->EnemyLoot, this->EnemyLootTable, this->MaximalAmountOfItems, PlayerReference->CurrentLevel);
}

void ABaseEnemy::Attack()
{
	this->bCanAttack = false;

	if(this->RangedWeapon != NULL)
	{
		this->RangedWeapon->FireOneBullet();
	}
	else
	{
		// Melee Attack
	}

	GetWorldTimerManager().SetTimer(AttackHandle, this, &ABaseEnemy::ResetAttack, 
									this->AttackCooldownTime, false);
}

void ABaseEnemy::ResetAttack()
{
	this->bCanAttack = true;

	GetWorldTimerManager().ClearTimer(AttackHandle);
}

void ABaseEnemy::DropPickups()
{
	if(this->UtilityPickupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UtilityPickup class not set in BaseEnemy."));
		return;
	}

	// Randomly pick how many pickups to spawn.
	int32 SpawnAmount = FMath::RandRange(1, this->MaximalPickupSpawn);
	AUtilityPickup* Pickup;

	while(SpawnAmount != 0)
	{
		Pickup = GetWorld()->SpawnActor<AUtilityPickup>(
			this->UtilityPickupClass, 
			this->GetActorLocation(), 
			FRotator(0, 0, 0)
		);

		if(!this->EnemyTicketMap.IsEmpty()) { Pickup->TicketMap = this->EnemyTicketMap; }

		// First we need to get the type since it determines the mesh,
		// then we initialize the mesh itself and do the drop effect.
		Pickup->Type = Pickup->RandomizePickupType();
		Pickup->InitializeUtilityMesh();
		Pickup->PickupDropEffect();
		
		SpawnAmount--;
	}
}

void ABaseEnemy::InitializeDeathTimer()
{
	if(this->RelevantQuest != NULL && this->RelevantQuest->Objectives[this->DefeatObjectiveIndex].bIsActive)
	{
		this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->DefeatObjectiveIndex]);
	}

	this->DropPickups();
	this->DropLoot();
	this->SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimer(DeathHandle, this, &ABaseEnemy::HandleDestruction, 3, false);

	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());

	if(EnemyController != nullptr)
	{
		EnemyController->PlayerCharacter->AddXP(this->ExperienceOnKill);
		EnemyController->OnUnPossess();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy controller is null."));
	}
}

void ABaseEnemy::HandleDestruction()
{
	if(this->RangedWeapon != NULL)
	{
		this->RangedWeapon->Destroy();
	}

	this->Destroy();
}
