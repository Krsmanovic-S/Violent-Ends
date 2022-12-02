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

public:

	void Explode();

	UPROPERTY(VisibleAnywhere, Category = "Grenade")
	UStaticMeshComponent* GrenadeMesh;

private:

	UPROPERTY(VisibleAnywhere, Category = "Grenade")
	USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float FuseLength = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	TSubclassOf<UBaseCustomDamageType> GrenadeDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float GrenadeDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	UParticleSystem* GrenadeExplosionEffect;
};