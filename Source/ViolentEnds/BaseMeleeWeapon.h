// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMeleeWeapon.generated.h"

class UBaseCustomDamageType;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeWeaponAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeWeaponAttackCooldownCleared);

class USkeletalMeshComponent;

UCLASS()
class VIOLENTENDS_API ABaseMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	  // Sets default values for this actor's properties
	ABaseMeleeWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	float MeleeCooldownDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	float Angle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	float MeleeRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	USkeletalMeshComponent* MeleeWeaponMesh;
  
	/* Melee attack function
	*  Return if the attack fails to happen
	 */
	bool Attack();

	/* Melee cooldown function, used as MeleeCooldownTimer ends
	*  Can also be called by owner objects if this was animation driven 
	*/
	UFUNCTION()
	void AttackReset();
	
	/* Used to be binded to the player or enemy pawn
	*  Called when the attack starts
	*  Can be binded to play melee montage 
	*/
	UPROPERTY(BlueprintAssignable, Category = "Offence Properties") 
	FMeleeWeaponAttack OnMeleeWeaponAttack ;
	
	/* Used to be binded to the player or enemy pawn
	*  Called when the cooldown is reset
	*  Can be binded to update UI indicates melee is ready to use
	*/
	UPROPERTY(BlueprintAssignable, Category = "Offence Properties") 
	FMeleeWeaponAttackCooldownCleared OnMeleeWeaponAttackCooldownCleared;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	TMap<TSubclassOf<UBaseCustomDamageType>, float> WeaponDamageTypes;

	// Used to lock melee
	bool bCanUseMelee;

	FTimerHandle MeleeCooldown;

	void DealDamage(AActor* Target);

#if WITH_EDITORONLY_DATA

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EDITOR")
	bool bShowMeleeDebug;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EDITOR")
	float DebugDuration;

#endif
};
