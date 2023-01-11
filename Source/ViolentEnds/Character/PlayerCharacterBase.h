#pragma once

#include "CoreMinimal.h"

#include "ViolentEnds/BaseAmmo.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"

#include "PlayerCharacterBase.generated.h"


class AMainPlayerController;
class ABaseGun;
class UInventoryComponent;
class UBaseQuest;
class UAIPerceptionStimuliSourceComponent;
class UEntityStats;
class UBaseItem;
class UBaseAmmo;
class IInteractiveObject;
class AGrenade;
class UBoxComponent;
class UAnimMontage;
class USoundBase;
class UWorldMapComponent;

UCLASS()
class VIOLENTENDS_API APlayerCharacterBase :
	public ACharacter
	//public IGenericTeamAgentInterface,
	//public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();
//
//protected:
//	virtual void BeginPlay() override;
//
//public:
//	virtual void Tick(float DeltaTime) override;
//
//	UFUNCTION(BlueprintCallable)
//	UWorld* GetWorldFromPlayer() const { return this->GetWorld(); };
//
//	// Equipping and Unequipping
//	void EquipWeapon();
//	void UnequipWeapon();
//	// ------------------------------------------
//	UFUNCTION(BlueprintCallable)
//	void AddXP(float& ExperienceToAdd);
//
//	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
//	void DropItem(UBaseItem* Item, bool bItemWasInInventory);
//
//	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
//	void AddInfoMessage(const FText& InfoMessageName);
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UWorldMapComponent* PlayerMap;
//
//	UPROPERTY(EditAnywhere)
//	TArray<USoundBase*> GunSounds;
//
//	AMainPlayerController* MainPlayerController;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	UAIPerceptionStimuliSourceComponent* PerceptionStimuliComp;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UInventoryComponent* PlayerInventory;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UEntityStats* PlayerStats;
//
//	UPROPERTY(BlueprintReadWrite)
//	ABaseGun* Gun;
//
//	UPROPERTY(BlueprintReadWrite)
//	float CurrentXP;
//
//	UPROPERTY(BlueprintReadWrite)
//	float XPForNextLevel;
//
//	UPROPERTY(BlueprintReadWrite)
//	int32 CurrentLevel = 1;
//
//	UPROPERTY(BlueprintReadWrite)
//	int32 AvailableSkillPoints = 0;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Animations")
//	UAnimMontage* ReloadAnimation;
//
//	UPROPERTY(BlueprintAssignable)
//	FLevelUp OnLevelUp;
//
//	UPROPERTY(BlueprintAssignable)
//	FAmmoUpdate OnAmmoUpdated;
//
//	void ReloadWeapon();
//
//	/** Can this character attack? */
//	bool CanAttack();
//
//	/** Can this character reload? */
//	bool CanReload();
//
//private:
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
//	// Character Movement and Aiming
//	void RotateCharacterToMouse(FVector LookAtTarget);
//	void Aim();
//	void StopAiming();
//	void MoveForward(float AxisValue);
//	void MoveRight(float AxisValue);
//	void Run();
//	void StopRunning();
//	/** Switches the current running desire */
//	void ToggleRunning();
//	void Dash();
//	void ResetDash();
//	// ------------------------------------------
//	// Shooting
//	void Attack();
//	void StopAttacking();
//	// ------------------------------------------
//	// Ammo & Weapon Swap
//	void SwapWeapons();
//	void EquipAmmo(EFiringStyle AmmoFireStyle);
//	void EquipStandardAmmo();
//	void EquipBurstAmmo();
//	void EquipShotgunAmmo();
//	void EquipSniperAmmo();
//	// ------------------------------------------
//	void ThrowGrenade();
//	// ------------------------------------------
//	// Interaction System
//	UFUNCTION()
//	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
//		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//	UFUNCTION()
//	void OnBoxEndOverlap(
//		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
//
//	void OnInteract();
//
//	virtual FGenericTeamId GetGenericTeamId() const override;
//
//	UPROPERTY(EditAnywhere)
//	UBoxComponent* InteractionZone;
//
//	TArray<AActor*> OverlappingInteractables;
//
//	IInteractiveObject* CurrentInteractable;
//	// ------------------------------------------
//	// XP & Leveling
//	void PlayerLevelUp();
//	// ------------------------------------------
//	// Grenade
//	AGrenade* Grenade;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Grenade", meta = (AllowPrivateAccess = true))
//	TSubclassOf<AGrenade> GrenadeClass;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player|Grenade", meta = (AllowPrivateAccess = true))
//	int32 GrenadeCount;
//	// ------------------------------------------
//	// Dash/Dodge
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Dashing", meta = (AllowPrivateAccess = true))
//	float DashDistance;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Dashing", meta = (AllowPrivateAccess = true))
//	float DashCooldown;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Dashing", meta = (AllowPrivateAccess = true))
//	float DashStaminaCost;
//	// ------------------------------------------
//
//	UPROPERTY(EditDefaultsOnly, Category = "Player|Ammo Types")
//	TArray<TSubclassOf<UBaseAmmo>> InitialClassesForAmmo;
//
//	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
//	bool bAllowedAmmoEquip = true;
//
//	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
//	bool bAllowedReload = true;
//
//	bool bAllowedAttack = true;
//	bool bIsAiming;
//	bool bHasDashed = false;
//
//	FTimerHandle DashHandle;
//	FTimerHandle InvincibilityHandle;
//	FHitResult MouseHitResult;
//
//public: // Quest
//	/**
//	 * Add a quest to the player's quest list
//	 * @param QuestToAdd the quest class to instantiate
//	 * @param bSetActive if enabled, will also mark it as the active one
//	 */
//	void AddQuest(TSubclassOf<UBaseQuest> QuestToAdd, bool bSetActive = false);
//
//	/**
//	 * Update the currently active quest
//	 * @param NewActiveQuest pointer to the quest in AllQuests
//	 */
//	void SetActiveQuest(UBaseQuest* NewActiveQuest);
//
//	/**
//	 * Find a quest of the given class in AllQuests
//	 * @param QuestToFind class of quest to look for
//	 * @return pointer to the quest if found, nullptr otherwise
//	 */
//	UBaseQuest* FindQuestByClass(TSubclassOf<UBaseQuest> QuestToFind);
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
//	TSubclassOf<UBaseQuest> TestingQuest;
//
//	UPROPERTY(BlueprintReadWrite)
//	TArray<UBaseQuest*> AllQuests;
//
//	UPROPERTY(BlueprintReadWrite)
//	UBaseQuest* CurrentActiveQuest;
//
//	UPROPERTY(BlueprintAssignable)
//	FQuestUpdate OnQuestUpdated;
//
//protected: // Movement Config
//	/* Speed (in Unreal units/second) the character runs at */
//	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
//	float RunSpeed = 600;
//
//	/* Speed (in Unreal units/second) the character walks at */
//	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
//	float WalkSpeed = 400;
//
//protected: // Dash
//	/* Duration to apply invincibility effect after dash is initiated, which blocks all damage being taken */
//	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dash")
//	float DashInvincibilityDuration = 0.15;
//
//public: // Gameplay Tags
//	/* Get a copy of all currently owned gameplay tags */
//	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
//
//protected:
//	/* Remove the Invincibility gameplay tag (Status.Invincibility) */
//	void RemoveInvincibility();
//
//	/* Container to hold all gameplay tags applied to the player */
//	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
//	FGameplayTagContainer GameplayTags;
};