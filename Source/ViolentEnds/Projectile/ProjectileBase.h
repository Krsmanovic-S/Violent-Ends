#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"

#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
class UGameplayEffect;

UCLASS()
class VIOLENTENDS_API AProjectileBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectileBase();

	FGameplayTag ImpactCueTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	UAbilitySystemComponent* OwnerAbilitySystemComponent;

	// Collision detection component
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	FGameplayEffectSpecHandle HitGameplayEffect;

	// Projectile movement component, used to adjust projectile properties 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Static mesh component 
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMeshComponent;

	// Pierce count: amount of object this projectile can move through 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	int32 PierceCount;

	// How many times can this projectile bounce on hit 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	int32 BounceCount;
	
	// Callback function when the projectile started overlap with anything
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
    // Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual FHitResult FindHitImpact();
};
