#include "UtilityPickup.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "EntityStats.h"
#include "InventoryComponent.h"
#include "BaseAmmo.h"
#include "Kismet/GameplayStatics.h"


AUtilityPickup::AUtilityPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	this->UtilityActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = this->UtilityActorMesh;
}

void AUtilityPickup::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if(this->PlayerReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player ref not set for UtilityPickup."));
		this->Destroy();
	}

	this->UtilityActorMesh->OnComponentBeginOverlap.AddDynamic(this, &AUtilityPickup::OnUtilityOverlap);
}

void AUtilityPickup::InitializeUtilityMesh()
{
	if(this->UtilityPossibleMeshes.Num() < 3) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Not all possible meshes set for Utility Actor"));
		return;
	}

	// Initialize the static mesh based on pickup type.
	switch(this->Type)
	{
		case EUtilityType::Health:
			this->UtilityActorMesh->SetStaticMesh(this->UtilityPossibleMeshes[0]);
			this->UtilityActorMesh->SetMaterial(0, this->TemporaryTestingMaterials[0]);
			return;
		case EUtilityType::Stamina:
			this->UtilityActorMesh->SetStaticMesh(this->UtilityPossibleMeshes[1]);
			this->UtilityActorMesh->SetMaterial(0, this->TemporaryTestingMaterials[1]);
			return;
		default:
			this->UtilityActorMesh->SetStaticMesh(this->UtilityPossibleMeshes[2]);
			this->UtilityActorMesh->SetWorldScale3D(FVector(1.2, 1.2, 1.2));
			return;
	}
}

EUtilityType AUtilityPickup::RandomizePickupType()
{
	TArray<uint8> AllTickets;
	uint8 TicketInteger = 0;

	for(auto& MapValue : this->TicketMap)
	{
		// Add this ticket as many times as the map specifies
		// and then change the ticket (just increment is fine).
		for(int32 i = 0; i < MapValue.Value; i++) { AllTickets.Add(TicketInteger); }
		TicketInteger++;
	}

	if(AllTickets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UtilityPickup RandomizePickupType() method error."));
		return EUtilityType::None;
	}

	// Pick one of the tickets at random from the above array.
	int32 RandomNumber = AllTickets[FMath::RandRange(0, AllTickets.Num() - 1)];

	switch(RandomNumber)
	{
		case 0: return EUtilityType::Health; 
		case 1: return EUtilityType::Stamina; 
		case 2: return EUtilityType::StandardAmmo;
		case 3: return EUtilityType::BurstAmmo; 
		case 4: return EUtilityType::ShotgunAmmo;
		case 5: return EUtilityType::SniperAmmo;
	}
	return EUtilityType::None;
}

void AUtilityPickup::ReplenishAmmo()
{
	float IncreaseAmount = 0;
	UBaseAmmo* AmmoItem;

	switch(this->Type)
	{
		case EUtilityType::StandardAmmo: 
			AmmoItem = this->PlayerReference->PlayerInventory->AmmoInventory[0];
			IncreaseAmount = AmmoItem->ItemMaxStack * (this->StandardAmmoAmount / 100);

			break;
		case EUtilityType::BurstAmmo:
			AmmoItem = this->PlayerReference->PlayerInventory->AmmoInventory[1];
			IncreaseAmount = AmmoItem->ItemMaxStack * (1 / this->BurstAmmoAmount);

			break;
		case EUtilityType::ShotgunAmmo:
			AmmoItem = this->PlayerReference->PlayerInventory->AmmoInventory[2];
			IncreaseAmount = AmmoItem->ItemMaxStack * (this->ShotgunAmmoAmount / 100);

			break;
		case EUtilityType::SniperAmmo:
			AmmoItem = this->PlayerReference->PlayerInventory->AmmoInventory[3];
			IncreaseAmount = AmmoItem->ItemMaxStack * (this->SniperAmmoAmount / 100);

			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Something went wrong in UtilityPickup."));
			return;
	}

	int32 IntegerIncrease = FMath::RoundToInt(IncreaseAmount);

	if(AmmoItem->ItemCurrentStack + IntegerIncrease > AmmoItem->ItemMaxStack)
	{
		AmmoItem->ItemCurrentStack = AmmoItem->ItemMaxStack;
	}
	else
	{
		AmmoItem->ItemCurrentStack += IntegerIncrease;
	}
}

void AUtilityPickup::OnUtilityOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only execute further if we overlapped the Player.
	if(!OtherActor->IsA<APlayerCharacter>()) { return; }

	float IncreaseAmount = 0;

	switch(this->Type)
	{
		case EUtilityType::Health:
			IncreaseAmount = this->PlayerReference->PlayerStats->MaximumHealth * (this->HealthReplenish / 100);
			this->PlayerReference->PlayerStats->FlatHeal(IncreaseAmount);

			break;
		case EUtilityType::Stamina:
			IncreaseAmount = this->PlayerReference->PlayerStats->MaximumStamina * (this->StaminaReplenish / 100);
			this->PlayerReference->PlayerStats->FlatStaminaAdd(IncreaseAmount);

			break;
		default:
			this->ReplenishAmmo();
			break;		
	}
	this->Destroy();
}
