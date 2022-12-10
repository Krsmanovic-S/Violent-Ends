#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Grenade.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UBaseCustomDamageType;
class UParticleSystem;

UCLASS()
class VIOLENTENDS_API AGrenade : public AActor
{
	GENERATED_BODY()

public:
	AGrenade();

protected:
	virtual void BeginPlay() override;

	/* Deals damage to any overlapping actor inside the damage radius */
	void GrenadeExplode();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GrenadeMesh;

private:
	/* Radius in which actors will get damaged by the grenade */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* DamageSphere;

	/* Time in seconds after which the grenade will explode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float FuseLength = 2.f;

	/* What type of damage will the grenade do upon explosion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	TSubclassOf<UBaseCustomDamageType> GrenadeDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float GrenadeDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	UParticleSystem* GrenadeExplosionEffect;
};