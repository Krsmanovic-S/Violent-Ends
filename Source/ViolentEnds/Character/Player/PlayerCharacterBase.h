#pragma once

#include "CoreMinimal.h"

#include "ViolentEnds/Character/CharacterBase.h"

#include "PlayerCharacterBase.generated.h"

class UCharacterInventoryComponent;
class UBoxComponent;

UCLASS()
class VIOLENTENDS_API APlayerCharacterBase : public ACharacterBase {

	GENERATED_BODY()

public:
	
	APlayerCharacterBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Inventory")
	UCharacterInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	UBoxComponent* InteractionDetection;

	UFUNCTION()
	void StartRotation();

	UFUNCTION()
	void EndRotation();

protected:

	void PossessedBy(AController* NewController) override;

private:

	UPROPERTY()
	APlayerController* PlayerController;

	FTimerHandle RotationTimer;

	bool bIsRotating;

	UFUNCTION()
	void CharacterRotationCallback();
};