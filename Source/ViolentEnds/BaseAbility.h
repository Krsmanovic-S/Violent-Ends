#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbility.generated.h"


UCLASS(BlueprintType, Blueprintable)
class VIOLENTENDS_API UBaseAbility : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	bool CanCastAbility(float& CurrentStamina , const bool bFreeCast = false);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CastAbility();

	UFUNCTION(BlueprintCallable)
	void StartAbilityCooldown();

    void ResetAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	class UTexture2D* AbilityThumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	FText AbilityDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	FText AbilityDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	float StaminaCostToCast = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	float Cooldown = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	float CastDuration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	bool bDisableMovementWhileCasting = false;

	bool bIsOnCooldown = false;

	FTimerHandle CooldownHandle;

};
