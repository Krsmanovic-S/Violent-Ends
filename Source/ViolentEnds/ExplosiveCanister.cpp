#include "ExplosiveCanister.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "EntityStats.h"
#include "BaseCustomDamageType.h"
#include "Kismet/GameplayStatics.h"


AExplosiveCanister::AExplosiveCanister()
{
	PrimaryActorTick.bCanEverTick = false;

	this->CanisterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CanisterMesh"));
	SetRootComponent(this->CanisterMesh);

	this->DamageRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageRadius"));
	this->DamageRadiusSphere->SetupAttachment(this->CanisterMesh);
}

void AExplosiveCanister::BeginPlay()
{
	Super::BeginPlay();
	
	if(!this->CanisterDamageType)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage Type Class for Explosive Canister is not set."));
		this->Destroy();

		return;
	}

	OnTakeAnyDamage.AddDynamic(this, &AExplosiveCanister::ExplodeOnHit);
}

void AExplosiveCanister::ExplodeOnHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorControler, AActor* DamageCauser)
{
	TArray<AActor*> OverlappingActors;
	FDamageEvent DamageEvent;
	float DamageToInflict = 1;

	this->SetCanBeDamaged(false);

	this->DamageRadiusSphere->GetOverlappingActors(OverlappingActors);

	for(AActor* Actor : OverlappingActors)
	{
		if(Actor->CanBeDamaged())
		{
			if(Actor->FindComponentByClass<class UEntityStats>())
			{
				DamageToInflict = this->CanisterDamageType.GetDefaultObject()->ReturnDamageAmount(
					Actor->FindComponentByClass<UEntityStats>(), this->CanisterDamageAmount);										
			}
			Actor->TakeDamage(DamageToInflict, DamageEvent, NULL, this);
		}
	}

	if(this->CanisterExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), this->CanisterExplosionEffect, this->GetActorTransform(), true);
	}

	this->Destroy();
}
