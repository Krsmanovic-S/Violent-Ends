#include "Door.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	this->DoorInteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Door Interaction Box"));
	SetRootComponent(this->DoorInteractionCollision);

	this->DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	this->DoorFrameMesh->SetupAttachment(this->DoorInteractionCollision);

	this->FrontKeypad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front Keypad"));
	this->FrontKeypad->SetupAttachment(this->DoorFrameMesh);

	this->BackKeypad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Keypad"));
	this->BackKeypad->SetupAttachment(this->DoorFrameMesh);

	this->PivotSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot Point"));
	this->PivotSceneComp->SetupAttachment(this->DoorFrameMesh);

	this->LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	this->RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));

	this->LeftDoorMesh->SetupAttachment(this->PivotSceneComp);
	this->RightDoorMesh->SetupAttachment(this->PivotSceneComp);

	this->InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(FName("Interaction Widget"));
	this->InteractionWidget->SetupAttachment(this->DoorFrameMesh);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// From the IInteractable Interface
	this->InteractionWidget->SetVisibility(false);
	this->bCanBeUsedAgain = true;
	this->bCanInteract = this->bCanBeOpened;
	this->Type = InteractableType::Door;

	this->UpdateKeypadMesh();
}

void ADoor::InteractPure()
{
	// This calls the blueprint event for interacting.
	IInteractiveObject::Execute_Interact(this);

	GetWorldTimerManager().SetTimer(this->DoorAutoClose, this, &ADoor::CloseDoor, 10, false);
}

void ADoor::UpdateKeypadMesh()
{
	if (this->KeypadMeshes.Num() == 2)
	{
		if (this->bCanBeOpened)
		{
			this->FrontKeypad->SetStaticMesh(this->KeypadMeshes[0]);
			this->BackKeypad->SetStaticMesh(this->KeypadMeshes[0]);
		}
		else
		{
			this->FrontKeypad->SetStaticMesh(this->KeypadMeshes[1]);
			this->BackKeypad->SetStaticMesh(this->KeypadMeshes[1]);
		}
	}
}

void ADoor::CloseDoor()
{
	GetWorldTimerManager().ClearTimer(this->DoorAutoClose);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	float DistanceToPlayer = FVector::Dist(this->GetActorLocation(), PlayerPawn->GetActorLocation());

	// We don't want the door auto closing when a player is very nearby,
	// this is a duck-tape solution for now but we need to figure it out since
	// the auto close might happen just as the player is moving through the door.
	if (DistanceToPlayer > 25.f) { IInteractiveObject::Execute_Interact(this); }
	else { GetWorldTimerManager().SetTimer(this->DoorAutoClose, this, &ADoor::CloseDoor, 15, false); }
}