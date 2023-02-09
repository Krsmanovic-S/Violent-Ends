#include "CollectableNote.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "ViolentEnds/PlayerCharacter.h"

ACollectableNote::ACollectableNote()
{
	PrimaryActorTick.bCanEverTick = false;

	this->NoteInteractiveCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveCollision"));
	SetRootComponent(this->NoteInteractiveCollision);

	this->NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoteMesh"));
	this->NoteMesh->SetupAttachment(this->NoteInteractiveCollision);

	this->InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(FName("InteractionWidget"));
	this->InteractionWidget->SetupAttachment(this->NoteMesh);
}

void ACollectableNote::BeginPlay()
{
	Super::BeginPlay();

	// From the IInteractable Interface
	this->InteractionWidget->SetVisibility(false);
	this->bCanBeUsedAgain = false;
	this->bCanInteract = true;
	this->Type = InteractableType::Note;

	this->NoteMesh->OnComponentBeginOverlap.AddDynamic(this, &ACollectableNote::OnMeshBeginOverlap);
}

void ACollectableNote::InteractPure()
{
	// This calls the blueprint event for interacting.
	IInteractiveObject::Execute_Interact(this);
}

void ACollectableNote::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<APlayerCharacter>()) { IInteractiveObject::Execute_Interact(this); }
}
