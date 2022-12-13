#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EntityStats.generated.h"

// Delegate for updating segments of the HP bar whenever MaxHP changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthBarUpdate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIOLENTENDS_API UEntityStats : public UActorComponent
{
	GENERATED_BODY()

public:
	UEntityStats();

	/* Function that is called whenever an actor with this component takes damage */
	UFUNCTION(BlueprintCallable)
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator,
		AActor* DamageCauser);

	/* Regenerate actor current health by the amount equal
	   to the health regen amount multiplied by DeltaTime
	*/
	void RegenerateHealth();

	/* Handles the regeneration/reduction of stamina based
	   on whether the owner actor is running
	*/
	void HandleStamina();

	/**
	 * Increases the health by a flat amount and checks
	   that the current health isn't over the maximum one

	 * @param InputValue how much to increase the health
	 */
	UFUNCTION(BlueprintCallable)
	void FlatHeal(UPARAM(ref) const float& InputValue);

	/**
	 * Increases the stamina by a flat amount and checks
	   that the current stamina isn't over the maximum one

	 * @param InputValue how much to increase the stamina
	 */
	UFUNCTION(BlueprintCallable)
	void FlatStaminaAdd(UPARAM(ref) const float& InputValue);

	/* Sets the maximum health to the input value
	   and calls the delegate that is responsible
	   for segmenting the health bar UI
	*/
	void SetMaximumHealth(const float& InputValue);

	/* Re-calculates how many segments of the health
	   bar do we need and updates the actual bar
	*/
	UPROPERTY(BlueprintAssignable)
	FHealthBarUpdate OnMaximumHealthUpdated;

protected:
	virtual void BeginPlay() override;

	/* Health and Stamina regeneration is based on DeltaTime,
	   so we do the neccessary calls here for both of them
	*/
	virtual void TickComponent(
		float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaximumHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthRegenerationAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bShouldRegenHealth;

	//-------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaximumStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenerationAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDecreaseAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	bool bIsEntityRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	bool bShouldRegenStamina;

	//-------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	int32 ArmorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	int32 FireResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	int32 ExplosiveResistance;

	//-------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offence")
	float GlobalDamageBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offence")
	float FireDamageBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offence")
	float CriticalHitChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offence")
	float CriticalHitDamageMultiplier = 1;

	//-------------------------------------------------------------------
};
