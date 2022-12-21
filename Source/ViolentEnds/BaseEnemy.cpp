#include "BaseEnemy.h"

#include "BaseGun.h"
#include "BaseItem.h"
#include "BaseMeleeWeapon.h"
#include "BaseQuest.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIController.h"
#include "EntityStats.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	this->EnemyStats = CreateDefaultSubobject<UEntityStats>(TEXT("Enemy Stats"));

	this->PerceptionStimuliComp =
		CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimulus"));

	// Prevents the pawn from snapping rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (this->BlueprintGunClass != NULL)
	{
		FActorSpawnParameters SpawnParameters;

		SpawnParameters.Owner = this;

		this->RangedWeapon = GetWorld()->SpawnActor<ABaseGun>(this->BlueprintGunClass, SpawnParameters);
		this->RangedWeapon->AttachToComponent(
			GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunAttachPoint"));
		bCanAttack = true;
	}
	if (MeleeWeaponClass)
	{
		FActorSpawnParameters SpawnParams;

		SpawnParams.Owner = this;

		MeleeWeapon = GetWorld()->SpawnActor<ABaseMeleeWeapon>(MeleeWeaponClass, SpawnParams);
		MeleeWeapon->AttachToComponent(
			GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunAttachPoint"));
	}

	APlayerCharacter* PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	this->InitializeLoot(
		this->EnemyLoot, this->EnemyLootTable, this->MaximalAmountOfItems, PlayerReference->CurrentLevel);
}

void ABaseEnemy::Attack()
{
	this->bCanAttack = false;

	if (this->RangedWeapon != nullptr)
	{
		this->RangedWeapon->FireOneBullet();
		GetWorldTimerManager().SetTimer(AttackHandle, this, &ABaseEnemy::ResetAttack, this->AttackCooldownTime, false);
	}
	else
	{
		// Melee Attack
		if (this->MeleeWeapon != nullptr) { MeleeWeapon->Attack(); }
	}
}

void ABaseEnemy::ResetAttack()
{
	this->bCanAttack = true;

	GetWorldTimerManager().ClearTimer(AttackHandle);
}

void ABaseEnemy::DropPickups()
{
	if (this->UtilityPickupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UtilityPickup class not set in BaseEnemy."));
		return;
	}

	// Randomly pick how many pickups to spawn.
	int32 SpawnAmount = FMath::RandRange(1, this->MaximalPickupSpawn);
	AUtilityPickup* Pickup;

	while (SpawnAmount != 0)
	{
		Pickup = GetWorld()->SpawnActor<AUtilityPickup>(
			this->UtilityPickupClass, this->GetActorLocation(), FRotator(0, 0, 0));

		if (!this->EnemyTicketMap.IsEmpty()) { Pickup->TicketMap = this->EnemyTicketMap; }

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
	if (this->RelevantQuest != NULL && this->RelevantQuest->Objectives[this->DefeatObjectiveIndex].bIsActive)
	{
		this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->DefeatObjectiveIndex]);
	}

	this->DropPickups();
	this->DropLoot();
	this->SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimer(DeathHandle, this, &ABaseEnemy::HandleDestruction, 3, false);

	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());

	if (EnemyController != nullptr)
	{
		EnemyController->PlayerCharacter->AddXP(this->ExperienceOnKill);
		EnemyController->OnUnPossess();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Enemy controller is null.")); }
}

void ABaseEnemy::HandleDestruction()
{
	if (this->RangedWeapon != NULL) { this->RangedWeapon->Destroy(); }

	this->Destroy();
}

bool ABaseEnemy::HasRangedWeapon() const
{
	return RangedWeapon ? true : false;
}

bool ABaseEnemy::HasMeleeWeapon() const
{
	return MeleeWeapon ? true : false;
}

bool ABaseEnemy::TryAttackRanged()
{
	if (!RangedWeapon) { return false; }
	Attack();
	return true;
}

bool ABaseEnemy::TryAttackMelee()
{
	if (!MeleeWeapon) { return false; }

	bool Success = MeleeWeapon->Attack();
	return Success;
}
