#include "Projectile.h"

#include "BaseGun.h"
#include "EntityStats.h"
#include "ExplosiveCanister.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	this->ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	this->ProjectileMovementComponent =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	this->ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileOverlap);

	this->GunOwner = Cast<ABaseGun>(GetOwner());

	this->ProjectilePierceAmount = this->GunOwner->BulletPierceAmount;
}

void AProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!this->GunOwner)
	{
		this->Destroy();
		return;
	}

	float ProjectileDamage = 1;

	// If we hit an explosive canister or an actor that can be damaged
	// (has an UEntityStats component) then we damage that actor and reduce our pierce.
	if (OtherActor && OtherActor != this && OtherActor != this->GunOwner && OtherActor->CanBeDamaged())
	{
		if (OtherActor->FindComponentByClass<UEntityStats>())
		{
			ProjectileDamage = this->GunOwner->CalculateDamage(OtherActor->FindComponentByClass<UEntityStats>());
		}

		UGameplayStatics::ApplyDamage(
			OtherActor, ProjectileDamage, this->GunOwner->GetInstigatorController(), this, UDamageType::StaticClass());

		this->ProjectilePierceAmount--;
	}

	if (this->ProjectilePierceAmount <= 0) { this->Destroy(); }
}
