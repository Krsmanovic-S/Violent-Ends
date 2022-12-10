#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Projectile.generated.h"

class ABaseGun;
class UProjectileMovementComponent;

UCLASS()
class VIOLENTENDS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	friend class ABaseGun;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	ABaseGun* GunOwner;

	int32 ProjectilePierceAmount = 0;
};
