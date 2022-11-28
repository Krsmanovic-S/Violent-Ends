#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseGun.h"
#include "EntityStats.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;

	ProjectileRange = 700.f;

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	StartingLocation = GetActorLocation();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Try to move this from the tick function.
	ProjectileRange = Cast<ABaseGun>(GetOwner())->MaximumRange;

	CurrentLocation = GetActorLocation();

	if(FVector::Dist(StartingLocation, CurrentLocation) >= ProjectileRange)
	{
		Destroy();
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	AActor* MyOwner = GetOwner();

	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInst = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if(OtherActor && OtherActor != this && OtherActor != MyOwner
	   && OtherActor->FindComponentByClass<class UEntityStats>())
	{
		ABaseGun* GunOwner = Cast<ABaseGun>(GetOwner());

		ProjectileDamage += GunOwner->CalculateDamage(OtherActor->FindComponentByClass<class UEntityStats>());

		UGameplayStatics::ApplyDamage(OtherActor, 
									  ProjectileDamage, 
									  MyOwner->GetInstigatorController(), 
									  this, 
									  UDamageType::StaticClass());
	}

	Destroy();
}