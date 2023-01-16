#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "InteractiveObject.h"

#include "Door.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS(BlueprintType, Blueprintable)
class VIOLENTENDS_API ADoor : public AActor, public IInteractiveObject
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void BeginPlay() override;

	/**
	 * C++ version of the interaction from the IInteractable interface,
	 * for doors this just calls the blueprint version of the interaction
	 * as doing the opening/closing is much easier there (using timelines).
	 */
	virtual void InteractPure() override;

	/**
	 * Should be called whenever the state of the lock
	 * changes so that the keypad updates automatically.
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateKeypadMesh();

	void CloseDoor();

private:
	/**
	 * Collision box that will be registered by
	 * the interaction zone from the player
	 */
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* DoorInteractionCollision;

	/**
	 * Pivot point for moving the left and right doorway,
	 * it's location is used to set the relative position
	 * of each door side upon interaction
	 */
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* PivotSceneComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* DoorFrameMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* FrontKeypad;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BackKeypad;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* RightDoorMesh;

	/**
	 * Small widget showing the interact input key
	 * in front of the door, used to visualy indicate
	 * when the player can interact with it
	 */
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InteractionWidget;

	/* Can this door be opened or not? Useful for mechanics like needing keys etc.. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = true))
	bool bCanBeOpened = true;

	/* Is this door opened or closed right now? */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsCurrentlyOpen = false;

	/**
	 * Array to hold one mesh for an unlockable door (blue pad)
	 * and one for a locked door (red pad)
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Door")
	TArray<UStaticMesh*> KeypadMeshes;

	FTimerHandle DoorAutoClose;
};
