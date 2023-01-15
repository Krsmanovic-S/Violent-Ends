#pragma once

#include "CoreMinimal.h"

#include "ViolentEnds/Character/CharacterBase.h"

#include "PlayerCharacterBase.generated.h"

class UCharacterInventoryComponent;
class UBoxComponent;
class UQuestComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class VIOLENTENDS_API APlayerCharacterBase : public ACharacterBase {

	GENERATED_BODY()

public:
	
	APlayerCharacterBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Inventory")
	UCharacterInventoryComponent* InventoryComponent;	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Inventory")
	UQuestComponent* QuestComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	UBoxComponent* InteractionDetection;	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Camera")
	USpringArmComponent* SpringArmComponent;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Camera")
	UCameraComponent* CameraComponent;

	UFUNCTION(BlueprintCallable, Category = "Character|Aim")
	void StartRotation();

	UFUNCTION(BlueprintCallable, Category = "Character|Aim")
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