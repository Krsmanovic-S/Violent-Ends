#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "ViolentEnds/InteractiveObject.h"

#include "CollectableNote.generated.h"

class APlayerCharacter;
class UBoxComponent;
class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class VIOLENTENDS_API ACollectableNote : public AActor, public IInteractiveObject
{
	GENERATED_BODY()

public:
	ACollectableNote();

protected:
	virtual void BeginPlay() override;

	/**
	 * C++ version of the interaction from the IInteractable interface,
	 * for notes this just calls the blueprint version of the interaction
	 * where we update the UI, and update variables for the Player BP.
	 */
	virtual void InteractPure() override;

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* NoteInteractiveCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* NoteMesh;

	/**
	 * Small widget showing the interact input key above the note
	 */
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InteractionWidget;
};
