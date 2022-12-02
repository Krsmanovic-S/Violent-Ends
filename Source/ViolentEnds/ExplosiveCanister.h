#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveCanister.generated.h"


class UStaticMeshComponent;
class USphereComponent;
class UBaseCustomDamageType;
class UParticleSystem;

UCLASS()
class VIOLENTENDS_API AExplosiveCanister : public AActor
{
	GENERATED_BODY()
	
public:	

	AExplosiveCanister();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ExplodeOnHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorControler, AActor* DamageCauser);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* CanisterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	USphereComponent* DamageRadiusSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	TSubclassOf<UBaseCustomDamageType> CanisterDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	float CanisterDamageAmount = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive Canister", meta = (AllowPrivateAccess = true))
	UParticleSystem* CanisterExplosionEffect;
};