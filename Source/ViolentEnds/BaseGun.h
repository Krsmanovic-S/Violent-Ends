#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FiringStyle.h"
#include "BaseGun.generated.h"


class AProjectile;
class UBaseCustomDamageType;
class APlayerCharacter;
class UEntityStats;


UCLASS()
class VIOLENTENDS_API ABaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseGun();

protected:
	virtual void BeginPlay() override;

public:	

	void FireOneBullet(FVector ProjectileDirection = {10000.0, 0.0, 0.0});
	void EnemyFireOneBullet(FVector ProjectileDirection = {10000.0, 0.0, 0.0});
	void FireBurst();
	void FireShotgun();
	void FireSniper();
	void EnableMovement();

	UFUNCTION(BlueprintCallable)
	bool CanReload();

	UFUNCTION(BlueprintCallable)
	void Reload();
    
	void PullTrigger();
	float CalculateDamage(UEntityStats* OtherEntity);

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();

	UPROPERTY(VisibleAnywhere, Category = "Gun Properties")
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Properties")
	TSubclassOf<AProjectile> ProjectileClass;

	// -------------------------------------------------------
	// Ammo
	UPROPERTY(BlueprintReadWrite)
	class UBaseAmmo* HeldAmmo;

	UPROPERTY(BlueprintReadWrite)
	float MaximumRange = 10.f;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite)
	int32 MagazineSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	TArray<EFiringStyle> AcceptableAmmoTypes;
	// -------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	TMap<TSubclassOf<UBaseCustomDamageType>, float> GunDamageTypes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Offence Properties")
	int32 BurstAmount;

	// -------------------------------------------------------
	
	FTimerHandle BurstFireHandle;
	FTimerHandle SniperHandle;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFiring;

	UPROPERTY(BlueprintReadWrite)
	float ReloadTime = 1.f;

private:

	void InitializeGunProperties();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	int32 ReserveAmmo;

	int32 DivideDamageAmount = 1;

	APlayerCharacter* OwningPlayer;

};
