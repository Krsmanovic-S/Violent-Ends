#include "BaseGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "EntityStats.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "BaseCustomDamageType.h"
#include "InventoryComponent.h"
#include "BaseItem.h"
#include "BaseAmmo.h"
#include "FiringStyle.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseEnemy.h"


ABaseGun::ABaseGun()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	GunMesh->SetupAttachment(Root);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(GunMesh);
}

void ABaseGun::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->IsA<APlayerCharacter>())
	{
		this->OwningPlayer = Cast<APlayerCharacter>(GetOwner());

		this->InitializeGunProperties();
	}
	// Aside from the Player, only enemies so far can carry guns.
	else
	{
		ABaseEnemy* OwningEnemy = Cast<ABaseEnemy>(GetOwner());

		this->MaximumRange = OwningEnemy->EnemyAttackRange;

		this->CurrentAmmo = 1;

		this->GunDamageTypes = OwningEnemy->EnemyDamageTypes;
	}
}

void ABaseGun::InitializeGunProperties()
{
	int32 WeaponIndex = this->OwningPlayer->PlayerInventory->WeaponSlotIndex;
	auto* WeaponItem = this->OwningPlayer->PlayerInventory->CurrentItems[WeaponIndex];

	this->GunDamageTypes = WeaponItem->ItemStats.ItemDamageTypes;
}

void ABaseGun::FireOneBullet(FVector ProjectileDirection)
{
	if(!GetOwner()->IsA<ABaseEnemy>() && (this->CurrentAmmo == 0 || this->HeldAmmo == NULL))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation(),
		SpawnParameters
	);

	Projectile->SetLifeSpan(this->MaximumRange / ProjectileDirection.X);

	Projectile->ProjectileMovementComponent->SetVelocityInLocalSpace(ProjectileDirection);

	if(!GetOwner()->IsA<ABaseEnemy>())
	{
		this->CurrentAmmo--;
		this->HeldAmmo->ItemCurrentStack--;
	}
}

void ABaseGun::FireBurst()
{
	this->FireOneBullet();
	
	this->BurstAmount--;

	if(this->BurstAmount > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(BurstFireHandle, this, &ABaseGun::FireBurst, 0.05, false);            
	}
	else
	{
		if(this->HeldAmmo != NULL)
		{
			this->BurstAmount = this->HeldAmmo->AmountToFire;
		}
		else
		{
			this->BurstAmount = 0;
		}

		GetWorldTimerManager().ClearTimer(BurstFireHandle);
	}
}

void ABaseGun::FireShotgun()
{
	float VelocityPitch = -3900;

	int32 FireAmount = this->HeldAmmo->AmountToFire;

	// For those cases when the Player doesn't have enough ammo for a full magazine.
	if(this->HeldAmmo->AmountToFire > this->HeldAmmo->ItemCurrentStack)
	{
		FireAmount = this->HeldAmmo->ItemCurrentStack;
	}

	for(int i = 0; i < FireAmount; i++)
	{
		// VelocityPitch is how we get a shotgun like pattern for bullets.
		this->FireOneBullet(FVector(7000, VelocityPitch, 0.0));

		VelocityPitch += 1300;
	}
}

void ABaseGun::FireSniper()
{
	if(this->OwningPlayer && this->OwningPlayer->GetCharacterMovement() && this->CurrentAmmo)
	{
		this->OwningPlayer->GetCharacterMovement()->SetMovementMode(MOVE_None, 0);

		GetWorldTimerManager().SetTimer(SniperHandle, this, &ABaseGun::EnableMovement, 0.25, false);
	}
}

void ABaseGun::EnableMovement()
{
	this->FireOneBullet(FVector(11000.0, 0.0, 0.0));

	if(this->OwningPlayer && this->OwningPlayer->GetCharacterMovement())
	{
		this->OwningPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking, 1);
	}
}

bool ABaseGun::CanReload()
{
	// This is called whenever the reload input key is pressed.
	if(this->HeldAmmo && this->ReserveAmmo && !bIsFiring
	   && this->CurrentAmmo != this->MagazineSize)
	{
		return true;
	}
	
	return false;
}

void ABaseGun::Reload()
{	
	if(this->ReserveAmmo > (this->MagazineSize - this->CurrentAmmo))
	{
		this->ReserveAmmo -= (this->MagazineSize - this->CurrentAmmo);

		CurrentAmmo = this->MagazineSize;
	}
	else
	{
		this->CurrentAmmo += this->ReserveAmmo;

		this->ReserveAmmo = 0;
	}
}

void ABaseGun::PullTrigger()
{
	if(!this->ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Forgot to assign Projectile Class in BP."));
		return;
	}

	if(this->HeldAmmo)
	{
		switch(this->HeldAmmo->AmmoFireStyle)
		{
			case EFiringStyle::Standard: 
				this->DivideDamageAmount = 1;
				this->FireOneBullet();
				break;
			case EFiringStyle::Burst:
				this->DivideDamageAmount = 2;
				this->FireBurst();
				break;
			case EFiringStyle::Shotgun:
				this->DivideDamageAmount = 3;
				this->FireShotgun();
				break;
			case EFiringStyle::Sniper:
				this->DivideDamageAmount = 1;
				this->FireSniper();
				break;
			default: break;
		}
	}
}

float ABaseGun::CalculateDamage(class UEntityStats* OtherEntity)
{
	float CurrentDamageFromType = 0;
	float ResultingDamage = 0;

	for(auto& DamageType : GunDamageTypes)
	{
		CurrentDamageFromType = DamageType.Key.GetDefaultObject()->ReturnDamageAmount(GetOwner()->FindComponentByClass<class UEntityStats>(), DamageType.Value);

		// Criticals are calculated from whomever fired the projectile.
		DamageType.Key.GetDefaultObject()->CriticalHit(GetOwner()->FindComponentByClass<class UEntityStats>(), CurrentDamageFromType);

		ResultingDamage += CurrentDamageFromType;
	}

	return ResultingDamage / this->DivideDamageAmount;
}

void ABaseGun::UpdateAmmo()
{
	this->CurrentAmmo = 0;

	if(this->HeldAmmo)
	{
		this->ReserveAmmo = this->HeldAmmo->ItemCurrentStack;

		this->MagazineSize = this->HeldAmmo->AmmoMagazineSize;

		this->BurstAmount = this->HeldAmmo->AmountToFire;

		this->MaximumRange = this->HeldAmmo->GunFiringRange;

		this->BulletPierceAmount = this->HeldAmmo->AmmoPierceAmount;
	}
	else
	{
		this->ReserveAmmo = 0;
	}

	

	this->OwningPlayer->OnAmmoUpdated.Broadcast();
}
