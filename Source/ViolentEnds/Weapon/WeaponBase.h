#pragma once

#include "CoreMinimal.h"
#include "WeaponFireMode.h"
#include "ViolentEnds/Equipment/EquipmentBase.h"

#include "WeaponBase.generated.h"

class UAmmoBase;
class UGameplayEffect;

UCLASS()
class VIOLENTENDS_API UWeaponBase: public UEquipmentBase {

	GENERATED_BODY()

public:
	UWeaponBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon|FireMode")
	EWeaponFireMode WeaponFireMode;

	/**
	* How much ammo does a single shooting action consume
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	int32 AmmoPerShot;

	/**
	* Shots per second
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float FireRate;

	/**
	* Maximum amount of ammo a magazine can hold
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	int32 MagazineSize;

	/**
	* The amount of ammo left in a magazine
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	int32 Reserve;	
	
	/**
	* The maximum range which aim can offset to
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float AimRange;	
	
	/**
	* The inaccuracy of the weapon, measured in max angle which the projectile can spawn different from the aim position
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float InaccuracySpreadAngle;

	// Effective range of the weapon
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float MaxDamageRange;

	// Effect used to grant abilities to give to the character in relevant to weapon gameplay weapon functionalities
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TSubclassOf<UGameplayEffect> WeaponAbilitiesGrantedEffect;

	// Primary use: Equip item
	virtual bool TryUseItem_Implementation(AActor* Target) override;

	// Secondary use:
	virtual bool TryUseItemSecondary_Implementation(AActor* Target) override;

	// Always return true
	virtual bool CanBeUsed_Implementation(AActor* Target) override;

	/**
	* Function used to be called after a shoot ability action successfully performed
	* This will reduce ammo accordingly
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ShootEffect();
	
	/**
	* Function used to be called after a reload action successfully performed
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReloadEffect(int32 AmmoReloaded);

	/**
	* Returns if weapon is out of ammo in magazine
	* @return True if weapon is out of ammo in magazine
	*/
	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsOutOfAmmo();	
	
	/**
	* Returns if weapon has enough ammo to shoot
	* @return True if weapon has enough ammo to shoot
	*/
	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool HasEnoughAmmoToShoot();	

};
