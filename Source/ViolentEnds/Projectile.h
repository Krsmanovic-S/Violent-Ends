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

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovementComponent;

private:

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	void DestroyProjectileAtMaxRange();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	ABaseGun* GunOwner;

	int32 ProjectilePierceAmount = 0;
};
