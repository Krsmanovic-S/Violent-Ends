#pragma once

#include "CoreMinimal.h"

#include "EnvironmentalHazardBase.h"

#include "ExplosiveBarrelBase.generated.h"

class UNiagaraSystem;
class UAudioComponent;
class USphereComponent;
class UGameplayEffect;
class UCurveFloat;
class UBoxComponent;

/**
* Base class for explosive barrels (and other types of explosive interactable) that activates itself when collided with a projectile
*/
UCLASS(Abstract)
class VIOLENTENDS_API AExplosiveBarrelBase : public AEnvironmentalHazardBase
{
	GENERATED_BODY()

public:
	AExplosiveBarrelBase();

	/**
	* The display mesh of the explosive prop
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UStaticMeshComponent* BarrelMeshComponent;

	/**
	* The physic collider of the prop
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UBoxComponent* BaseCollision;

	/**
	* The collision of the explosive prop
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	USphereComponent* ActivationDetection;

	/**
	* The SFX/VFX effect spawn location
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	USceneComponent* VisualEffectSpawnLocation;

	/**
	* The effect to play when this is activated
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UNiagaraSystem* ExplosionVFX;

	/**
	* The sound effect when this is activated
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UAudioComponent* ExplosionAudioComponent;

	/**
	* The gameplay effect used to apply to the targets in range
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	TSubclassOf<UGameplayEffect> ExplosionEffect;

	/**
	* The damaging range, everything within this range will be accounted for dealing damage
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	float DamageRange;

	/**
	* Optional: the damage falloff based on the distance of target(s) from this object
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UCurveFloat* DamageFalloff;


	UFUNCTION(BlueprintNativeEvent, Category = "Hazard")
	void OnActivationDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:

	virtual void OnActivationDetected_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void Activate() override;
	virtual bool CanBeReused() override;
	virtual void OnActivated_Implementation() override;
};