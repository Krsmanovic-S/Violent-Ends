#include "ItemActor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemBase.h"
#include "ViolentEnds/Inventory/CharacterInventoryComponent.h"

AItemActor::AItemActor()
{
	CollisionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollisionBox"));
	CollisionDetection->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	SetRootComponent(CollisionDetection);
	Mesh->SetupAttachment(GetRootComponent());
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	// Throw item up
	FVector Impulse = FVector();
	Impulse.Z = FMath::RandRange(0.966f, 1.f);
	Impulse.X = FMath::RandRange(0.f, .5f);
	Impulse.Y = FMath::RandRange(0.f, .5f);

	// CollisionDetection->AddImpulse(Impulse);
}

UItemBase* AItemActor::GetItem() const
{
	return ItemHeld;
}

void AItemActor::InteractWithObject(AActor* Source)
{
	// What wants to interact with this needs to have Inventory Component
	if (!Source) { return; }

	UE_LOG(LogTemp, Display, TEXT("%s attempts to interact with %s"), *Source->GetFName().ToString(),
		*GetFName().ToString())

	auto InventoryComponent = Source->FindComponentByClass<UCharacterInventoryComponent>();
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction source doesn't have an inventory component"))
		return;
	}
	int32 OutIndex;
	bool Success = InventoryComponent->TryAddItemToInventory(GetItem(), OutIndex);
	if (Success)
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Interaction successful"))
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Interaction unsuccessful"))
}
