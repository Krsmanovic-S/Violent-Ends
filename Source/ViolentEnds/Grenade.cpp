#include "Grenade.h"

#include "BaseCustomDamageType.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EntityStats.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = false;

	this->DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Sphere"));
	RootComponent = this->DamageSphere;

	this->GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	this->GrenadeMesh->SetupAttachment(this->DamageSphere);
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (!this->GrenadeDamageType)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage Type Class for Grenades is not set."));
		return;
	}

	FTimerHandle ExplodeHandle;
	// Set the delay after which the grenade will explode.
	GetWorldTimerManager().SetTimer(ExplodeHandle, this, &AGrenade::GrenadeExplode, FuseLength, false);
}

void AGrenade::GrenadeExplode()
{
	TArray<AActor*> OverlappingActors;

	this->DamageSphere->GetOverlappingActors(OverlappingActors);

	FDamageEvent DamageEvent;
	float DamageToInflict = 1;
	this->SetCanBeDamaged(false);

	// For the enemies we do a for loop and apply the damage to all of them.
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->CanBeDamaged())
		{
			if (Actor->FindComponentByClass<UEntityStats>())
			{
				DamageToInflict = this->GrenadeDamageType.GetDefaultObject()->ReturnDamageAmount(
					Actor->FindComponentByClass<UEntityStats>(), this->GrenadeDamage);
			}
			Actor->TakeDamage(DamageToInflict, DamageEvent, NULL, this);
		}
	}

	if (this->GrenadeExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), this->GrenadeExplosionEffect, this->GetActorTransform(), true);
	}

	this->Destroy();
}
