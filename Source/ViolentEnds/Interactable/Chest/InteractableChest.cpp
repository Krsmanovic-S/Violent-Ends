#include "InteractableChest.h"
#include "Components/BoxComponent.h"
#include "ViolentEnds/Item/ItemActor.h"

AInteractableChest::AInteractableChest() {
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	InteractableArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableArea"));
	ItemSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawn"));

	ChestMesh->SetupAttachment(GetRootComponent());
	InteractableArea->SetupAttachment(GetRootComponent());
	ItemSpawnLocation->SetupAttachment(GetRootComponent());
}

void AInteractableChest::OpenChest_Implementation() {
	UWorld* WorldRef = GetWorld();
	FTransform SpawnTransform = ItemSpawnLocation->GetComponentTransform();
	for (int32 i = 0; i < ContainedItems.Num(); i++) {
		
		AItemActor* NewActor = WorldRef->SpawnActorDeferred<AItemActor>(AItemActor::StaticClass(), SpawnTransform);
		if (NewActor) {

			NewActor->ItemHeld = ContainedItems[i];
			NewActor->FinishSpawning(SpawnTransform);
		}
	}
}

void AInteractableChest::InteractWithObject(AActor* Source) {

}
