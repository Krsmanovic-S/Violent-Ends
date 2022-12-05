#include "ExplosiveCanister.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "EntityStats.h"
#include "BaseCustomDamageType.h"
#include "EnvironmentalDamageActor.h"
#include "Components/BoxComponent.h"
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
			// If the overlapped actor has EntityStats, then calculate the
			// damage, otherwise just apply the damage of 1 (for i.e. other canisters, props..)
			if(Actor->FindComponentByClass<class UEntityStats>())
			{
				DamageToInflict = this->CanisterDamageType.GetDefaultObject()->ReturnDamageAmount(
					Actor->FindComponentByClass<UEntityStats>(), this->CanisterDamageAmount);										
			}
			Actor->TakeDamage(DamageToInflict, DamageEvent, NULL, this);
		}
	}

	if(this->CanisterExplosionEffect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), this->CanisterExplosionEffect, this->GetActorTransform(), true);
	}

	if(this->ActorToSpawnOnDeath != nullptr)
	{
		// We randomize the Y rotation of the actor just to have a little bit of variety.
		float RandomYRotation = FMath::RandRange(-90.f, 90.f);
		AEnvironmentalDamageActor* ENV_DamageActor = GetWorld()->SpawnActor<AEnvironmentalDamageActor>(this->ActorToSpawnOnDeath, 
																	   this->GetActorLocation(), FRotator(0, RandomYRotation, 0));

		// Life span is randomized according to our member variable here.
		float RandomLifeSpan = FMath::RandRange(this->LifeSpanRange.X, this->LifeSpanRange.Y);
		ENV_DamageActor->SetLifeSpan(RandomLifeSpan);

		ENV_DamageActor->OverlapZone->SetBoxExtent(this->EnvironmentalDamageBoxExtent);
	}

	this->Destroy();
}
