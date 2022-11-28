#include "BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseGun.h"
#include "EntityStats.h"
#include "BaseItem.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "TimerManager.h"
#include "BaseQuest.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	this->PerceptionStimuliComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimulus"));

	this->EnemyStats = CreateDefaultSubobject<UEntityStats>(TEXT("Enemy Stats"));
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

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::Attack()
{
	if(this->bCanAttack)
	{
		this->bCanAttack = false;

		if(this->RangedWeapon != NULL)
		{
			this->RangedWeapon->EnemyFireOneBullet();
		}
		else
		{
			// Melee Attack
		}

		GetWorldTimerManager().SetTimer(AttackHandle, this, &ABaseEnemy::ResetAttack, 
							this->AttackCooldownTime, false);
	}
}

void ABaseEnemy::ResetAttack()
{
	this->bCanAttack = true;

	GetWorldTimerManager().ClearTimer(AttackHandle);
}

void ABaseEnemy::InitializeDeathTimer()
{
	if(this->RelevantQuest != NULL && this->RelevantQuest->Objectives[this->DefeatObjectiveIndex].bIsActive)
	{
		this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->DefeatObjectiveIndex]);
	}

	this->DropLoot();

	GetWorldTimerManager().SetTimer(DeathHandle, this, &ABaseEnemy::HandleDestruction, 3, false);

	this->SetActorEnableCollision(false);
}

void ABaseEnemy::HandleDestruction()
{
	if(this->RangedWeapon != NULL)
	{
		this->RangedWeapon->Destroy();
	}

	this->Destroy();
}