#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "FiringStyle.h"
#include "PlayerCharacter.generated.h"


// Delegate that will be used to update the Pause Menu.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUp);

// Delegate that Blueprints will use to update the Ammo UI.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoUpdated);

// Delegate that Blueprints will use to update the Qeuest Widget.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestUpdated);

class AMainPlayerController;
class ABaseGun;
class UInventoryComponent;
class UBaseQuest;
class UAIPerceptionStimuliSourceComponent;
class UEntityStats;
class UBaseItem;
class IInteractiveObject;
class AGrenade;
class UBoxComponent;
class UAnimMontage;


UCLASS()
class VIOLENTENDS_API APlayerCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Equipping and Unequipping
	void EquipWeapon();
	void UnequipWeapon();
	// ------------------------------------------
	void AddXP(float& ExperienceToAdd);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DropItem(UBaseItem* Item, bool bItemWasInInventory);

	AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UEntityStats* PlayerStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TSubclassOf<UBaseQuest> TestingQuest;

	UPROPERTY(BlueprintReadWrite)
	TArray<UBaseQuest*> AllQuests;

	UPROPERTY(BlueprintReadWrite)
	UBaseQuest* CurrentActiveQuest;

	UPROPERTY(BlueprintReadWrite)
	ABaseGun* Gun;

	UPROPERTY(BlueprintReadWrite)
	float CurrentXP;

	UPROPERTY(BlueprintReadWrite)
	float XPForNextLevel;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentLevel = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 AvailableSkillPoints = 0;

	bool bIsInCombat = false;

	UPROPERTY(BlueprintReadWrite)
	bool bAllowedAmmoEquip = true;

	UPROPERTY(BlueprintReadWrite)
	bool bAllowedReload = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Animations")
	UAnimMontage* ReloadAnimation;

	FTimerHandle FireHandle;
	FTimerHandle ShootingHandle;

	UPROPERTY(BlueprintAssignable)
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable)
	FOnAmmoUpdated OnAmmoUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;

	void ReloadWeapon();

private:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Character Movement and Aiming
	void RotateCharacterToMouse(FVector LookAtTarget);
	void Aim();
	void StopAiming();
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Run();
	void StopRunning();
	void Dash();
	void ResetDash();
	// ------------------------------------------
	// Shooting
	void Attack();
	void StopAttacking();
	void ResetShootingCooldown();
	// ------------------------------------------
	// Ammo
	void EquipAmmo(EFiringStyle AmmoFireStyle);
	void EquipStandardAmmo();
	void EquipBurstAmmo();
	void EquipShotgunAmmo();
	void EquipSniperAmmo();

	uint8 CurrentlyEquippedAmmo = 0;

	// ------------------------------------------
	void ThrowGrenade();
	// ------------------------------------------
	// Interaction System
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void OnInteract();

	UPROPERTY(EditAnywhere)
	UBoxComponent* InteractionZone;

	TArray<AActor*> OverlappingInteractables;

	IInteractiveObject* CurrentInteractable;
	// ------------------------------------------
	// XP & Leveling
	void PlayerLevelUp();
	// ------------------------------------------
	// Grenade
	AGrenade* Grenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Grenade", meta = (AllowPrivateAccess = true))
	TSubclassOf<AGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player|Grenade", meta = (AllowPrivateAccess = true))
	int32 GrenadeCount;
	// ------------------------------------------
	// Dash/Dodge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Dashing", meta = (AllowPrivateAccess = true))
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Dashing", meta = (AllowPrivateAccess = true))
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Dashing", meta = (AllowPrivateAccess = true))
	float DashStaminaCost;
	// ------------------------------------------

	virtual FGenericTeamId GetGenericTeamId() const override;

	bool bIsAiming;
	bool bHasDashed = false;

	FTimerHandle DashHandle;
	FHitResult MouseHitResult;
};
