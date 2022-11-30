#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LootSystem.h"
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
	virtual void Tick(float DeltaTime) override;

	void Attack();
	void ResetAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void DropLoot();

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

	bool bCanAttack;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle AttackHandle;

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ABaseGun* RangedWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	float AttackCooldownTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseGun> BlueprintGunClass;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	TArray<UBaseItem*> EnemyLoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess = true))
	UDataTable* EnemyLootTable;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy", BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 MaximalAmountOfItems = 1;

	FTimerHandle DeathHandle;
};
