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

/**
* The base class for projectile
* Default behaviour: Pierce if possible, if cannot pierce then it will bounce if possible
*/
UCLASS()
class VIOLENTENDS_API AProjectileBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectileBase();

	/**
	* The gameplay tag used to represent the impact of this projectile when it collides with a different object
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	FGameplayTag ImpactCueTag;

	/**
	* The owner that created this projectile
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"), Category = "Projectile")
	UAbilitySystemComponent* OwnerAbilitySystemComponent;

	// Collision detection component
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"), Category = "Projectile")
	FGameplayEffectSpecHandle HitGameplayEffect;

	// Projectile movement component, used to adjust projectile properties 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Static mesh component 
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileMeshComponent;

	// Pierce count: amount of object this projectile can move through 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"), Category = "Projectile")
	int32 PierceCount;

	// How many times can this projectile bounce on hit 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"), Category = "Projectile")
	int32 BounceCount;
	
	// Callback function when the projectile started overlap with anything
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
    // Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void DisableProjectile();	
	
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void EnableProjectile();


protected:

	virtual FHitResult FindHitImpact();
	virtual void DisableProjectile_Implementation();
	virtual void EnableProjectile_Implementation();
};
