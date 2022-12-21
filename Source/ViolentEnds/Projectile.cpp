#include "Projectile.h"

#include "BaseEnemy.h"
#include "BaseGun.h"
#include "EntityStats.h"
#include "ExplosiveCanister.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Shield.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseEvent_Damage.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	this->ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(this->ProjectileMesh);

	this->ProjectileMovementComponent =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	this->ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileOverlap);
	this->OnActorHit.AddDynamic(this, &AProjectile::OnProjectileBlockingHit);

	this->GunOwner = Cast<ABaseGun>(GetOwner());

	this->ProjectilePierceAmount = this->GunOwner->BulletPierceAmount;

	if (this->GunOwner->bShouldProjectilesBounce) { this->ProjectileMovementComponent->bShouldBounce = true; }
}

void AProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!this->GunOwner)
	{
		this->Destroy();
		return;
	}

	if (OtherActor->IsA<AShield>())
	{
		AShield* ShieldActor = Cast<AShield>(OtherActor);

		// Players bullets should ignore his own Shield.
		if (this->GunOwner->GetOwner()->IsA<APlayerCharacter>() && ShieldActor->bIgnorePlayerShield)
		{
			this->bShouldSpawnParticles = false;
			return;
		}

		// Enemy bullets should ignore their own Shield.
		if (this->GunOwner->GetOwner()->IsA<ABaseEnemy>() && ShieldActor->bIgnoreEnemyShield)
		{
			this->bShouldSpawnParticles = false;
			return;
		}
	}

	float ProjectileDamage = 1;

	// If we hit an actor that can be damaged, we first look if we should calculate the damage
	// (actor has an UEntityStats component), then we damage that actor and reduce our pierce.
	if (OtherActor && OtherActor != this && OtherActor != this->GunOwner && OtherActor->CanBeDamaged())
	{
		if (OtherActor->FindComponentByClass<UEntityStats>())
		{
			ProjectileDamage = this->GunOwner->CalculateDamage(OtherActor->FindComponentByClass<UEntityStats>());
		}

		UGameplayStatics::ApplyDamage(
			OtherActor, ProjectileDamage, this->GunOwner->GetInstigatorController(), this, UDamageType::StaticClass());

		// Report Damage Sense to the AI Perception System
		UAISense_Damage::ReportDamageEvent(OtherActor, OtherActor, this->GunOwner, 1.0, this->GunOwner->GetActorLocation(), Hit.Location);
		
		// Currently obsolete as the assets look bad compared to the blueprint call.
		if (this->OverlapImpactEffect != nullptr && this->bShouldSpawnParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), this->OverlapImpactEffect, this->GetActorTransform(), true);
		}

		this->ProjectilePierceAmount--;
	}

	this->SpawnParticleEffect();

	if (this->ProjectilePierceAmount <= 0) { this->Destroy(); }
}

void AProjectile::OnProjectileBlockingHit(
	AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Currently obsolete as the assets look bad compared to the blueprint call.
	if (this->HitImpactEffect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), this->HitImpactEffect, this->GetActorTransform(), true);
	}

	if (this->PossibleBounceHits != 0) { this->PossibleBounceHits--; }
	else { this->Destroy(); }
}