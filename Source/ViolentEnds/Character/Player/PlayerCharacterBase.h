#pragma once

#include "CoreMinimal.h"

#include "ViolentEnds/Character/CharacterBase.h"

#include "PlayerCharacterBase.generated.h"

class UCharacterInventoryComponent;
class UBoxComponent;
class UQuestComponent;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;

UCLASS()
class VIOLENTENDS_API APlayerCharacterBase : public ACharacterBase
{
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Interactable")
	TArray<AActor*> InteractablesInRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Interactable")
	AActor* CurrentInteractable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Interactable")
	UWidgetComponent* InteractionWidget;

	UFUNCTION(BlueprintCallable, Category = "Character|Interaction")
	void InteractWithItem();

	UFUNCTION(BlueprintNativeEvent, Category = "Character|Interation")
	void HighlightInteraction();

	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent, Category = "Character|Inventory|Weapon")
	void OnWeaponSheathed(UWeaponBase* Weapon);

	UFUNCTION(BlueprintNativeEvent, Category = "Character|Inventory|Weapon")
	void OnWeaponSwapped(UWeaponBase* OldWeapon, UWeaponBase* NewWeapon);

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaSecond) override;

	virtual void HighlightInteraction_Implementation();

	virtual void OnWeaponSheathed_Implementation(UWeaponBase* Weapon);
	virtual void OnWeaponSwapped_Implementation(UWeaponBase* OldWeapon, UWeaponBase* NewWeapon);

private:
	UPROPERTY()
	APlayerController* PlayerController;

	bool bIsAiming;

	UFUNCTION()
	void CharacterRotationCallback();
};