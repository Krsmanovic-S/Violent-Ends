#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "BaseAbility.generated.h"

class UTexture2D;
class APlayerCharacter;

UCLASS(BlueprintType, Blueprintable)
class VIOLENTENDS_API UBaseAbility : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool CanCastAbility(UPARAM(ref) float& CurrentStamina, const bool bFreeCast = false);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CastAbility(APlayerCharacter* CastingPlayer);

	UFUNCTION(BlueprintCallable)
	void StartAbilityCooldown();

	void ResetAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	UTexture2D* AbilityThumbnail;

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

	/* How long will the effect of this ability be active (in seconds) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	float AbilityDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	bool bDisableMovementWhileCasting = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsOnCooldown = false;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle CooldownHandle;
};
