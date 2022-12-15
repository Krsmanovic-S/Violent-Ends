#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "BaseMeleeWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeWeaponAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeWeaponAttackCooldownCleared);

class USkeletalMeshComponent;
class UBaseCustomDamageType;

UCLASS()
class VIOLENTENDS_API ABaseMeleeWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseMeleeWeapon();

	/** If not reset using animation notifies then this will be used for reseting the melee 
	* Defaults to infinity (will never be called) 
	* Needs to set in editor or use animation binding
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	float MeleeCooldownDuration;
	
	/** Dead zone: when the range is less than this radius, the hit will not connect 
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	float DeadZoneRadius;

	/** The angle (in degrees) which the weapon can hit 
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	float Angle;
	
	/** The maximum distance which the hit can connect to the target
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	float MeleeRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	USkeletalMeshComponent* MeleeWeaponMesh;

	/** 
	 * Melee attack function

	 * @return whether the attack fails to happen or not
	 */
	bool Attack();

	/**
	* Calculate the validity of the targets and apply the hit to the target if they are valid
	* If in animation mode, bind this function to the "hit" animation notify
	*/
	void HitTargets();

	/** Melee cooldown function, used as MeleeCooldownTimer ends
	 *  Can also be called by owner objects if this was animation driven
	 */
	UFUNCTION()
	void AttackReset();

	/** Used to be binded to the player or enemy pawn
	 *  Called when the attack starts
	 *  Can be binded to play melee montage
	 */
	UPROPERTY(BlueprintAssignable, Category = "Offence Properties")
	FMeleeWeaponAttack OnMeleeWeaponAttack;

	/** Used to be binded to the player or enemy pawn
	 *  Called when the cooldown is reset
	 *  Can be binded to update UI indicates melee is ready to use
	 */
	UPROPERTY(BlueprintAssignable, Category = "Offence Properties")
	FMeleeWeaponAttackCooldownCleared OnMeleeWeaponAttackCooldownCleared;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	TMap<TSubclassOf<UBaseCustomDamageType>, float> WeaponDamageTypes;

	/** 
	* Used to lock the attack action
	* NOTE: Can also be used as a lock to the hit logic so the hit cannot be used if the attack action hasn't been called
	*/
	bool bCanUseMelee;

	/**
	* Timer used to lock the attack action
	*/
	FTimerHandle MeleeCooldown;

	void DealDamage(AActor* Target);

#if WITH_EDITORONLY_DATA

public:
	/**
	* If enabled, draw a simple cone that indicates the attack trace
	* Editor only
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EDITOR")
	bool bShowMeleeDebug;

	/**
	* Duration of the debug if bShowMeleeDebug is enable, 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EDITOR")
	float DebugDuration;

#endif
};
