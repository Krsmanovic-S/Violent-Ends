#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentalDamageActor.generated.h"


class UBoxComponent;
class UBaseCustomDamageType;
class UParticleSystem;
class UEntityStats;

UCLASS(BlueprintType, Blueprintable)
class VIOLENTENDS_API AEnvironmentalDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentalDamageActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Actor")
	UBoxComponent* OverlapZone;

protected:
	virtual void BeginPlay() override;

	void ApplyDamageOverTime();


private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Actor", meta = (AllowPrivateAccess = true))
	UParticleSystem* EnvironmentalParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Actor", meta = (AllowPrivateAccess = true))
	TSubclassOf<UBaseCustomDamageType> EnvironmentalDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Actor", meta = (AllowPrivateAccess = true))
	float DamageToDeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Actor", meta = (AllowPrivateAccess = true))
	float TimeToDealDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Actor", meta = (AllowPrivateAccess = true))
	float LifeTime = 10.f;

	TArray<AActor*> OverlappingActors;

	FTimerHandle DamageHandle;
};
