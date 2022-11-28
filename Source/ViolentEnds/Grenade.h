#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"


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
	class UStaticMeshComponent* GrenadeMesh;

private:

	UPROPERTY(VisibleAnywhere, Category = "Grenade")
	class USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float FuseLength = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float ExplosiveDamage = 20.f;

};
