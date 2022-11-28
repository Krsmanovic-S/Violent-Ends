#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntityStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIOLENTENDS_API UEntityStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEntityStats();

	UFUNCTION(BlueprintCallable)
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

	void HandleHealth();

	void HandleStamina();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

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
