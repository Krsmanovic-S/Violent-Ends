#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LootSystem.h"
#include "UtilityPickup.h"
#include "BaseEnemy.generated.h"


class ABaseGun;
class UBaseCustomDamageType;
class UBaseItem;
class UEntityStats;
class UDataTable;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class VIOLENTENDS_API ABaseEnemy : public ACharacter, public ILootSystem
{
	GENERATED_BODY()
	
public:	
	ABaseEnemy();

protected:
	virtual void BeginPlay() override;

public:	

	void Attack();
	void ResetAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void DropLoot();

	UFUNCTION(BlueprintCallable)
	void DropPickups();

	void InitializeDeathTimer();
	void HandleDestruction();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	UEntityStats* EnemyStats;

	class UBaseQuest* RelevantQuest;

	int32 DefeatObjectiveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemyAttackRange = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	TMap<TSubclassOf<UBaseCustomDamageType>, float> EnemyDamageTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float WalkMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float RunMovementSpeed;

	bool bCanAttack;

	UPROPERTY(BlueprintReadWrite)
	ABaseGun* RangedWeapon;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseGun> BlueprintGunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	float AttackCooldownTime = 1.f;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	TArray<UBaseItem*> EnemyLoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	UDataTable* EnemyLootTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	int32 MaximalAmountOfItems = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	float ExperienceOnKill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|Pickups", meta = (AllowPrivateAccess = true))
	TSubclassOf<AUtilityPickup> UtilityPickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Pickups", meta = (AllowPrivateAccess = true))
	int32 MaximalPickupSpawn = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Pickups", meta = (AllowPrivateAccess = true))
	TMap<EUtilityType, int32> EnemyTicketMap;

	FTimerHandle AttackHandle;
	FTimerHandle DeathHandle;
};
