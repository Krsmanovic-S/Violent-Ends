#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Projectile.generated.h"

class ABaseGun;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class VIOLENTENDS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	friend class ABaseGun;

protected:
	virtual void BeginPlay() override;

	/* Event for spawning particles on hit (the current assets look very bad
	   with a call from C++ so we do it here) */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnParticleEffect();

	/* Binding function for what happens when the projectile mesh overlaps something */
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	/* Binding function for what happens when the projectile actor hits something */
	UFUNCTION()
	void OnProjectileBlockingHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	/* Gun which shot this projectile, this is set on projectile spawn in the gun class */
	ABaseGun* GunOwner;

	/* How many entities can this projectile overlap before being destroyed */
	int32 ProjectilePierceAmount = 0;

	/* How many times can this projectile bounce on hit */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 PossibleBounceHits = 3;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bShouldSpawnParticles = true;

	/* Effect that plays on a blocking hit */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Effects")
	UParticleSystem* HitImpactEffect;

	/* Effect that plays on an overlapping hit */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Effects")
	UParticleSystem* OverlapImpactEffect;
};
