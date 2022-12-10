#pragma once

#include "CoreMinimal.h"

#include "FiringStyle.h"
#include "GameFramework/Actor.h"

#include "BaseGun.generated.h"

class AProjectile;
class UBaseAmmo;
class UBaseCustomDamageType;
class APlayerCharacter;
class UEntityStats;

UCLASS()
class VIOLENTENDS_API ABaseGun : public AActor
{
	GENERATED_BODY()

public:
	ABaseGun();

	friend class APlayerCharacter;
	friend class AProjectile;

	/* Shoot one bullet and lower the ammo if the Player called.
	 *   Projectile velocity is defaulted but can be specified as well
	 */
	void FireOneBullet(FVector ProjectileDirection = { 9000.0, 0.0, 0.0 });

protected:
	virtual void BeginPlay() override;

protected:
	void FireBurst();
	void FireShotgun();
	void FireSniper();
	void EnableMovement();

	/* Calls a shooting method depending on the currently equipped ammo */
	void PullTrigger();

	/* This is called whenever the reload input key is pressed */
	UFUNCTION(BlueprintCallable)
	bool CanReload();

	/* Actual action of the reload, fired at the end of the reload animation */
	UFUNCTION(BlueprintCallable)
	void Reload();

	/* Called by the Projectile to calculate the damage, this is done by
	 *   calling the ReturnDamageAmount() method for each damage type of the gun
	 */
	float CalculateDamage(UEntityStats* OtherEntity);

	/* Called from the Player class upon equipping a new type of ammo */
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();

	/* Copy over the damage values from the associated weapon item.
	 * 	Only fired if the owner of the gun is the Player. */
	void InitializeGunDamage();

private:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Gun Properties")
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Properties", meta = (AllowPrivateAccess = true))
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBaseAmmo* HeldAmmo;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MaximumRange = 10.f;

	/* How long will the reload animation take to complete in seconds */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float ReloadTime = 1.f;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 BulletPierceAmount;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 MagazineSize;

	int32 DivideDamageAmount = 1;

	int32 BurstAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties", meta = (AllowPrivateAccess = true))
	TArray<EFiringStyle> AcceptableAmmoTypes;

	/* Damage types that the projectiles will deal upon impact, as well as their damage values */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties", meta = (AllowPrivateAccess = true))
	TMap<TSubclassOf<UBaseCustomDamageType>, float> GunDamageTypes;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsFiring;

	APlayerCharacter* OwningPlayer;

	FTimerHandle BurstFireHandle;
	FTimerHandle SniperHandle;
};
