#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "PlayerCharacter.h"
#include "BaseEnemy.h"
#include "EntityStats.h"
#include "Kismet/GameplayStatics.h"


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

	FTimerHandle ExplodeHandle;

	// Set the delay after which the grenade will explode.
	GetWorldTimerManager().SetTimer(ExplodeHandle, this, &AGrenade::Explode, FuseLength, false);
}

void AGrenade::Explode()
{
	//this->DamageSphere->SetWorldLocation(this->GrenadeMesh->GetComponentLocation());

	TArray<AActor*> OverlappingEnemies;
	TArray<AActor*> OverlappingPlayer;

	this->DamageSphere->GetOverlappingActors(OverlappingEnemies, ABaseEnemy::StaticClass());
	this->DamageSphere->GetOverlappingActors(OverlappingPlayer, APlayerCharacter::StaticClass());

	FDamageEvent DamageEvent;

	// For the enemies we do a for loop and apply the damage to all of them.
	for(AActor* Actor : OverlappingEnemies)
	{
		if(Actor->FindComponentByClass<class UEntityStats>())
		{
			Actor->TakeDamage(this->ExplosiveDamage, DamageEvent, NULL, this);
		}
	}

	// We know that there is only one Player, so no need for a for-loop.
	if(!OverlappingPlayer.IsEmpty())
	{
		OverlappingPlayer[0]->TakeDamage(this->ExplosiveDamage, DamageEvent, NULL, this);
	}

	this->Destroy();
}

