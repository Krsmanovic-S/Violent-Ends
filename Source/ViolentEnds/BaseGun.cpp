#include "BaseGun.h"

#include "BaseAmmo.h"
#include "BaseCustomDamageType.h"
#include "BaseEnemy.h"
#include "BaseItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "EntityStats.h"
#include "FiringStyle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "ViolentEnds/GlobalTags.h"

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

	if (GetOwner()->IsA<APlayerCharacter>())
	{
		this->OwningPlayer = Cast<APlayerCharacter>(GetOwner());

		this->InitializeGunDamage();
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

void ABaseGun::FireOneBullet(FVector ProjectileDirection)
{
	if (GetOwner()->IsA<APlayerCharacter>() && this->CurrentAmmo == 0) { return; }

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), SpawnParameters);

	if (Projectile)
	{
		Projectile->SetLifeSpan(this->MaximumRange / ProjectileDirection.X);
		Projectile->ProjectileMovementComponent->SetVelocityInLocalSpace(ProjectileDirection);
	}

	if (!GetOwner()->IsA<ABaseEnemy>())
	{
		this->CurrentAmmo--;
		this->HeldAmmo->ItemCurrentStack--;
	}
}

void ABaseGun::FireBurst()
{
	this->FireOneBullet();

	this->BurstAmount--;

	if (this->BurstAmount > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(BurstFireHandle, this, &ABaseGun::FireBurst, 0.1, false);
	}
	else
	{
		if (this->HeldAmmo != nullptr) { this->BurstAmount = this->HeldAmmo->AmountToFire; }
		else { this->BurstAmount = 0; }

		GetWorldTimerManager().ClearTimer(BurstFireHandle);
	}
}

void ABaseGun::FireShotgun()
{
	float VelocityPitch = -3900;

	int32 FireAmount = this->HeldAmmo->AmountToFire;

	// For those cases when the Player doesn't have enough ammo for a full magazine.
	if (this->HeldAmmo->AmountToFire > this->HeldAmmo->ItemCurrentStack)
	{
		FireAmount = this->HeldAmmo->ItemCurrentStack;
	}

	for (int i = 0; i < FireAmount; i++)
	{
		// VelocityPitch is how we get a shotgun like pattern for bullets.
		this->FireOneBullet(FVector(7000, VelocityPitch, 0.0));

		VelocityPitch += 1300;
	}
}

void ABaseGun::FireSniper()
{
	if (this->OwningPlayer && this->OwningPlayer->GetCharacterMovement() && this->CurrentAmmo)
	{
		this->OwningPlayer->GetCharacterMovement()->SetMovementMode(MOVE_None, 0);

		GetWorldTimerManager().SetTimer(SniperHandle, this, &ABaseGun::EnableMovement, 0.25, false);
	}
}

void ABaseGun::EnableMovement()
{
	this->FireOneBullet(FVector(11000.0, 0.0, 0.0));

	if (this->OwningPlayer && this->OwningPlayer->GetCharacterMovement())
	{
		this->OwningPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking, 1);
	}
}

void ABaseGun::PullTrigger()
{
	if (!this->ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Forgot to assign Projectile Class in BP."));
		return;
	}

	if (this->CurrentAmmo)
	{
		switch (this->HeldAmmo->AmmoFireStyle)
		{
			case EFiringStyle::Standard:
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OwningPlayer->GunSounds[0], GetActorLocation());
				this->DivideDamageAmount = 1;
				this->FireOneBullet();
				break;
			case EFiringStyle::Burst:
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OwningPlayer->GunSounds[1], GetActorLocation());
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
			default:
				break;
		}
	}
	else { this->OwningPlayer->ReloadWeapon(); }
}

bool ABaseGun::CanReload()
{
	if (this->HeldAmmo != nullptr)
	{
		if (this->HeldAmmo->ItemCurrentStack == 0)
		{
			this->OwningPlayer->AddInfoMessage(FText::FromString(TEXT("Insufficient Ammo")));
			return false;
		}
		if (this->CurrentAmmo != this->MagazineSize && (!bIsFiring || this->CurrentAmmo == 0)) { return true; }
	}

	return false;
}

void ABaseGun::Reload()
{
	if (this->HeldAmmo->ItemCurrentStack > (this->MagazineSize - this->CurrentAmmo))
	{
		CurrentAmmo = this->MagazineSize;
	}
	else { this->CurrentAmmo += this->HeldAmmo->ItemCurrentStack; }
}

float ABaseGun::CalculateDamage(class UEntityStats* OtherEntity)
{
	float CurrentDamageFromType = 0;
	float ResultingDamage = 0;

	for (auto& DamageType : GunDamageTypes)
	{
		CurrentDamageFromType = DamageType.Key.GetDefaultObject()->ReturnDamageAmount(
			GetOwner()->FindComponentByClass<UEntityStats>(), DamageType.Value);

		// Criticals are calculated from whomever fired the projectile.
		DamageType.Key.GetDefaultObject()->CriticalHit(
			GetOwner()->FindComponentByClass<UEntityStats>(), CurrentDamageFromType);

		ResultingDamage += CurrentDamageFromType;
	}

	return ResultingDamage / this->DivideDamageAmount;
}

void ABaseGun::UpdateAmmo()
{
	this->CurrentAmmo = 0;

	if (this->HeldAmmo)
	{
		this->MagazineSize = this->HeldAmmo->AmmoMagazineSize;

		this->BurstAmount = this->HeldAmmo->AmountToFire;

		this->MaximumRange = this->HeldAmmo->GunFiringRange;

		this->BulletPierceAmount = this->HeldAmmo->AmmoPierceAmount;
	}

	this->OwningPlayer->OnAmmoUpdated.Broadcast();
}

void ABaseGun::InitializeGunDamage()
{
	int32 WeaponIndex = this->OwningPlayer->PlayerInventory->WeaponSlotIndex;
	auto* WeaponItem = this->OwningPlayer->PlayerInventory->CurrentItems[WeaponIndex];

	this->GunDamageTypes = WeaponItem->ItemStats.ItemDamageTypes;
}

void ABaseGun::StartFiring()
{
	bIsFiring = true;

	bShouldProjectilesBounce = OwningPlayer->HasMatchingGameplayTag(FGlobalTags::Get().Weapon_BouncyBullets);

	HandleFiring();
}

void ABaseGun::StopFiring()
{
	bIsFiring = false;
}

void ABaseGun::HandleFiring()
{
	if (HeldAmmo == nullptr) { return; }

	if (HeldAmmo->ItemCurrentStack <= 0 && CanReload())
	{
		OwningPlayer->ReloadWeapon();

		// Schedule firing again after the reload is completed
		GetWorldTimerManager().SetTimer(FiringHandle, this, &ABaseGun::HandleFiring, ReloadTime, false);
		return;
	}

	if (!bIsFiring) { return; }

	if (OwningPlayer->CanAttack()) { PullTrigger(); }

	// Schedule fire again according to rate of fire
	// This will also end up retrying to attack if all checks passed, but the player was unable to attack
	//   EG due to a stun
	GetWorldTimerManager().SetTimer(FiringHandle, this, &ABaseGun::HandleFiring, 1 / HeldAmmo->ShotsPerSecond, false);
}