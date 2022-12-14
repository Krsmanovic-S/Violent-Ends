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

	/* Override that allows us to spawn actors via ability BPs */
	virtual UWorld* GetWorld() const { return this->World; };

	/* Assigns our member World from the one of the input pawn,
	   called when we assign an ability to a hotkey slot
	*/
	UFUNCTION(BlueprintCallable)
	void InitializeWorldFromPlayer(APawn* PlayerPawn);

	/** 
	 * Checks if we have enough stamina to cast or if it is a free cast
	 * @return true if we have stamina or it is free, otherwise false
	 */
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

	/* How long do we have to wait to cast this ability again (in seconds) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	float Cooldown = 1.0f;

	/* Time it takes to perform the cast, only relevant for channel abilites */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	float CastDuration = 0.f;

	/* How long will the effect of this ability be active (in seconds) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	float AbilityDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Properties")
	bool bDisableMovementWhileCasting = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsOnCooldown = false;

	UPROPERTY(Transient)
	UWorld* World;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle CooldownHandle;
};
