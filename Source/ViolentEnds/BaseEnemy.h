#pragma once

#include "CoreMinimal.h"

// Interfaces
#include "LootSystem.h"
#include "WeaponSystem.h"

#include "GameFramework/Character.h"
#include "UtilityPickup.h"

#include "BaseEnemy.generated.h"

class ABaseGun;
class ABaseMeleeWeapon;
class UBaseCustomDamageType;
class UBaseItem;
class UEntityStats;
class UDataTable;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class VIOLENTENDS_API ABaseEnemy : public ACharacter, public ILootSystem, public IWeaponSystem
{
	GENERATED_BODY()

public:
	ABaseEnemy();

protected:
	virtual void BeginPlay() override;

public:
	/* Calls the PullTrigger method if the enemy is attacking with
	   a ranged weapon, sets bCanAttack to false and sets a timer
	   for the ResetAttack function depending on the AttackCooldownTime */
	void Attack();

	/* Sets the bCanAttack to true so the enemy can attack again */
	void ResetAttack();

	/* Same drop effect for items as the player, chest, pickups ecc.. */
	UFUNCTION(BlueprintImplementableEvent)
	void DropLoot();

	/* Spawns and randomizes pickups upon death, first we randomize
	   the amount to spawn and then for each of them we randomize
	   its type and call their drop effect function */
	UFUNCTION(BlueprintCallable)
	void DropPickups();

	/* Update a quest if this enemy is relevant to it, drop the loot and the pickups, handles the
	   destruction of the AI Controller and sets a timer for when this actor should be destroyed.
	   This timer is there just so we can first play a death animation*/
	void InitializeDeathTimer();

	/* Destroys the ranged weapon if the enemy had one and then destroys this actor*/
	void HandleDestruction();

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
	
	UPROPERTY(BlueprintReadWrite)
	ABaseMeleeWeapon* MeleeWeapon;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UEntityStats* EnemyStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliComp;

	/* If set, determines which gun will the enemy spawn and use,
	   RangedWeapon variable will point to the created gun */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseGun> BlueprintGunClass;	
	
	/* The base class of the melee weapon, ranged weapon take precedence over melee weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseMeleeWeapon> MeleeWeaponClass;

	/* How often, in seconds, can this enemy attack? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	float AttackCooldownTime = 1.f;

	/* Table from which we will pick items for the enemy loot,
	   only items that are in the table are valid to be picked */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	UDataTable* EnemyLootTable;

	/* Actual loot of the enemy that will drop upon its death, generated
	   by the ILootSystem interface from which this class inherits */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UBaseItem*> EnemyLoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	int32 MaximalAmountOfItems = 1;

	/* How much XP will the enemy give upon being slain? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	float ExperienceOnKill;

	/* What class of AUtilityPickup will the enemy try to spawn upon death */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|Pickups", meta = (AllowPrivateAccess = true))
	TSubclassOf<AUtilityPickup> UtilityPickupClass;

	/* Largest amount of pickups the enemy can spawn, range is between 1 and this value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Pickups", meta = (AllowPrivateAccess = true))
	int32 MaximalPickupSpawn = 1;

	/* Custom influence on the pickup randomization. Overwrites the default
	   tickets for the pickup and uses them for drawing out the type on spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Pickups", meta = (AllowPrivateAccess = true))
	TMap<EUtilityType, int32> EnemyTicketMap;

	FTimerHandle AttackHandle;
	FTimerHandle DeathHandle;

	// Inherited via IWeaponSystem
	virtual bool HasRangedWeapon() const override;
	virtual bool HasMeleeWeapon() const override;
	virtual bool TryAttackRanged() override;
	virtual bool TryAttackMelee() override;
};
