#pragma once

#include "CoreMinimal.h"

#include "BaseAmmo.h"
#include "GameFramework/Actor.h"

#include "BaseGun.generated.h"

class AProjectile;
class UBaseAmmo;
class UBaseCustomDamageType;
class APlayerCharacter;
class UEntityStats;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class VIOLENTENDS_API ABaseGun : public AActor
{
	GENERATED_BODY()

public:
	ABaseGun();

	friend class APlayerCharacter;
	friend class AProjectile;

	/**
	 * Shoot one bullet and lower the ammo if the Player called.
	 * Projectile velocity is defaulted but can be specified as well

	 * @param ProjectileDirection default value for the projectile velocity
	 */
	void FireOneBullet(FVector ProjectileDirection = { 9000.0, 0.0, 0.0 });

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	void FireBurst();
	void FireShotgun();
	void FireSniper();
	void EnableMovement();

	/* Calls a shooting method depending on the currently equipped ammo */
	void PullTrigger();

	/* Called whenever the reload input key is pressed,
	   checks if we have ammo to reload with, is current
	   ammo less than the magazine and are we not firing
	*/
	UFUNCTION(BlueprintCallable)
	bool CanReload();

	/* Actual action of the reload, fired at the end of the reload animation */
	UFUNCTION(BlueprintCallable)
	void Reload();

	/* Called by the Projectile to calculate the damage, this is done by
	 * calling the ReturnDamageAmount() method for each damage type of the gun
	 */
	float CalculateDamage(UEntityStats* OtherEntity);

	UFUNCTION(BlueprintCallable)
	void DisplayLaserSight();

	/* Initializes the magazine size, burst amount, projectile pierce and
	   maximum range depending on the held ammo. Also updates the ammo widget UI
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();

	/* Copy over the damage values from the associated weapon item.
	   Only fired if the owner of the gun is the Player
	*/
	void InitializeGunDamage();

private:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Gun Properties")
	USkeletalMeshComponent* GunMesh;

	/** Place where the projectiles will spawn upon shooting */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* ProjectileSpawnPoint;

	/** What class of projectile will this weapon spawn upon being fired */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Properties", meta = (AllowPrivateAccess = true))
	TSubclassOf<AProjectile> ProjectileClass;

	/**
	 *  Starting point of the trace for the
	 *  laser, changes each time the laser
	 *  bounces if the trace hits something
	 */
	FVector LaserStart;

	/**
	 *  Ending point of the trace for the laser,
	 *  starts at gun max range and changes if
	 *  the trace hits something and the laser bounces
	 */
	FVector LaserEnd;

	/**
	 * How long will the laser beam be?
	 * Determined by the maximum range.
	 */
	float DistanceToCover;

	/**
	 * What ammo item is currently loaded in the gun,
	 * only relevant for the Player character
	 */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBaseAmmo* HeldAmmo;

	/**
	 * Determines the lenght that the projectile fired from this
	 * weapon can travel before geting destroyed automatically
	 */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MaximumRange = 10.f;

	/** How long will the reload animation take to complete in seconds */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float ReloadTime = 1.f;

	/**
	 * How many entities can bullets fired from this weapon pass through
	 * before they are destroyed. This value is passed on the projectile
	 * itself and all the logic is done in that class, not here
	 */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 BulletPierceAmount;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 MagazineSize;

	/**
	 * By how much should we divide the damage of a single projectile.
	 * Used to customize the damage when firing multiple bullets at the
	 * same time (i.e. burst or shotgun rounds)
	 */
	int32 DivideDamageAmount = 1;

	int32 BurstAmount;

	/**
	 * What types of ammo can this weapon use? Takes a firing style
	 * to determine which ammo (that has that same style) can be
	 * equipped and which can not be equipped
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties", meta = (AllowPrivateAccess = true))
	TArray<EFiringStyle> AcceptableAmmoTypes;

	/** Damage types that the projectiles will deal upon impact, as well as their damage values */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties", meta = (AllowPrivateAccess = true))
	TMap<TSubclassOf<UBaseCustomDamageType>, float> GunDamageTypes;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsFiring;

	/** Showing the laser only depends on the bouncy bullets ability for now */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bShowLaserSight = false;

	/** Assigned by the Player if he has the appropriate gameplay tag (Weapon.BouncyBullets) */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bShouldProjectilesBounce = false;

	APlayerCharacter* OwningPlayer;

	FTimerHandle BurstFireHandle;
	FTimerHandle SniperHandle;

public: // Weapon firing
	/** Begin firing sequence, till StopFiring is called, or the ammo runs out */
	void StartFiring();

	/** Stop the firing sequence */
	void StopFiring();

protected:
	FTimerHandle FiringHandle;

	/**
	 * Handle firing logic
	 * Will keep firing until StopFiring is called
	 * If the ammo runs out, can reload and continue firing
	 */
	void HandleFiring();
};