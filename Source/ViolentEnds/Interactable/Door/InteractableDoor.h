#pragma once

#include "CoreMinimal.h"
#include "ViolentEnds/Interactable/InteractableInterface.h"
#include "InteractableDoor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed,
	Closing,
	Opened,
	Opening,
};


UCLASS()
class VIOLENTENDS_API AInteractableDoor: public AActor, public IInteractableInterface {

	GENERATED_BODY()

public:

	AInteractableDoor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	UBoxComponent* InteractableArea;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	EDoorState DoorState;

	UFUNCTION(BlueprintNativeEvent)
	void DoorInteract();

	void DoorInteract_Implementation();

	// Inherited via IInteractableInterface
	virtual void InteractWithObject(AActor* Source) override;
};