#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ExplosiveCanister.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UBaseCustomDamageType;
class UParticleSystem;
class AEnvironmentalDamageActor;

UCLASS()
class VIOLENTENDS_API AExplosiveCanister : public AActor
{
	GENERATED_BODY()

public:
	AExplosiveCanister();

protected:
	virtual void BeginPlay() override;

	/* Deals damage to any overlapping actor inside the damage radius */
	UFUNCTION(BlueprintCallable)
	void ExplodeOnHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatorControler, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* CanisterMesh;

	/* Radius in which the canister will deal damage upon explosion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	USphereComponent* DamageRadiusSphere;

	/* What type of damage will the canister deal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	TSubclassOf<UBaseCustomDamageType> CanisterDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	float CanisterDamageAmount = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	UParticleSystem* CanisterExplosionEffect;

	/* Depending on if this is set in blueprints, an environmental
	   actor will be spawned after the canister explodes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister|Environmental",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<AEnvironmentalDamageActor> ActorToSpawnOnDeath;

	/* How long will the environmental actor, that is spawned
	   upon canister death, be present for (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister|Environmental",
		meta = (AllowPrivateAccess = true))
	FVector2D LifeSpanRange;

	/* Box range for the damage zone of the environmental actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister|Environmental",
		meta = (AllowPrivateAccess = true))
	FVector EnvironmentalDamageBoxExtent;
};