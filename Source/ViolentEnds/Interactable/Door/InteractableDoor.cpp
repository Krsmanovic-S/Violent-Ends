#include "InteractableDoor.h"
#include "Components/BoxComponent.h"

AInteractableDoor::AInteractableDoor() {

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	InteractableArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableArea"));

	USceneComponent* NewRoot = CreateDefaultSubobject<USceneComponent>(TEXT("NewRoot"));
	SetRootComponent(NewRoot);

	DoorMesh->SetupAttachment(GetRootComponent());
	InteractableArea->SetupAttachment(GetRootComponent());
}

void AInteractableDoor::DoorInteract_Implementation() {}

void AInteractableDoor::InteractWithObject(AActor* Source) {
	DoorInteract();
}
