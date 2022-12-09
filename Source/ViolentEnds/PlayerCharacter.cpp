#include "PlayerCharacter.h"

#include "Animation/AnimMontage.h"
#include "BaseAmmo.h"
#include "BaseGun.h"
#include "BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "EntityStats.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Grenade.h"
#include "InteractiveObject.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "NativeGameplayTags.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "TimerManager.h"
#include "ViolentEnds/GlobalTags.h"
#include "ViolentEnds/LogMacros.h"

// Height difference from center of player character to the gun's barrel
float GGun_Height = 55;

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	this->PlayerStats = CreateDefaultSubobject<UEntityStats>(TEXT("Player Stats"));

	this->PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));

	this->PerceptionStimuliComp =
		CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimulus"));

	this->InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Zone"));
	this->InteractionZone->SetupAttachment(this->GetMesh());
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->MainPlayerController = Cast<AMainPlayerController>(GetController());

	UBaseAmmo* AmmoItem;
	uint8 AmmoInventoryIndex = 0;

	// Initialize the UBaseAmmo for the AmmoInventory.
	for (auto& AmmoClass : this->InitialClassesForAmmo)
	{
		AmmoItem = Cast<UBaseAmmo>(AmmoClass->GetDefaultObject());

		// Start with 100 of each ammo, just for debugging purposes.
		AmmoItem->ItemCurrentStack = 100;

		AmmoItem->OwningInventory = this->PlayerInventory;
		AmmoItem->World = GetWorld();

		this->PlayerInventory->AmmoInventory[AmmoInventoryIndex] = AmmoItem;
		AmmoInventoryIndex++;
	}

	this->bIsAiming = false;
	this->bHasDashed = false;
	this->DashDistance = 2500.f;
	this->DashCooldown = 1.f;
	this->XPForNextLevel = 10;

	this->InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBoxBeginOverlap);
	this->InteractionZone->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnBoxEndOverlap);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MainPlayerController && this->bIsAiming)
	{
		MainPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseHitResult);

		// The mouse hit result occurs at some arbitrary point, from the camera to the floor/ terrain
		// Interpolate or extrapolate that trace to derive the point
		// where the line's Z value is equal to the player's gun height (roughly, player location Z + 55)
		// The player's location there is its half height, so it would be around 135cm above the current floor
		FVector HitLocation = MouseHitResult.Location;
		FVector CameraLocation = MouseHitResult.TraceStart;
		FVector TraceDirection = (CameraLocation - HitLocation).GetSafeNormal();
		float PlayerGunHeight = GetActorLocation().Z + GGun_Height;
		float HitToPlayerGunDistance = PlayerGunHeight - HitLocation.Z;
		FVector OffsetToLineIntersection = HitToPlayerGunDistance * (1 / TraceDirection.Z) * TraceDirection;

		FVector Target = HitLocation + OffsetToLineIntersection;

		RotateCharacterToMouse(Target);
	}

	if (this->PlayerStats->bIsEntityRunning && this->PlayerStats->CurrentStamina > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &APlayerCharacter::StopRunning);

	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &APlayerCharacter::Aim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &APlayerCharacter::StopAiming);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayerCharacter::StopAttacking);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Released, this, &APlayerCharacter::ReloadWeapon);

	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &APlayerCharacter::Dash);

	PlayerInputComponent->BindAction(TEXT("ThrowGrenade"), IE_Pressed, this, &APlayerCharacter::ThrowGrenade);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &APlayerCharacter::OnInteract);

	PlayerInputComponent->BindAction(TEXT("SwapWeapons"), IE_Pressed, this, &APlayerCharacter::SwapWeapons);

	PlayerInputComponent->BindAction(TEXT("StandardAmmo"), IE_Pressed, this, &APlayerCharacter::EquipStandardAmmo);
	PlayerInputComponent->BindAction(TEXT("BurstAmmo"), IE_Pressed, this, &APlayerCharacter::EquipBurstAmmo);
	PlayerInputComponent->BindAction(TEXT("ShotgunAmmo"), IE_Pressed, this, &APlayerCharacter::EquipShotgunAmmo);
	PlayerInputComponent->BindAction(TEXT("SniperAmmo"), IE_Pressed, this, &APlayerCharacter::EquipSniperAmmo);
}

// -------------------------Character Movement And Rotation To Cursor-------------------------
void APlayerCharacter::RotateCharacterToMouse(FVector LookAtTarget)
{
	// Look at target is the mouse hit result position.
	FVector ToTarget = LookAtTarget - GetMesh()->GetComponentLocation();

	// Vector math to get the new vector (from the destination to where we want it to start).
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw - 90.f, 0.f);

	GetMesh()->SetWorldRotation(FMath::RInterpTo(
		GetMesh()->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 7.5));
}

void APlayerCharacter::Aim()
{
	this->bIsAiming = true;
}

void APlayerCharacter::StopAiming()
{
	this->bIsAiming = false;
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::Run()
{
	this->PlayerStats->bIsEntityRunning = true;
}

void APlayerCharacter::StopRunning()
{
	this->PlayerStats->bIsEntityRunning = false;
}

void APlayerCharacter::Dash()
{
	if (this->PlayerStats->CurrentStamina > DashStaminaCost && GetCharacterMovement()->Velocity != FVector::ZeroVector)
	{
		const FVector DashDirection = this->GetCharacterMovement()->GetLastInputVector();

		LaunchCharacter(DashDirection * this->DashDistance, true, true);

		this->PlayerStats->CurrentStamina -= DashStaminaCost;
		GameplayTags.AddTag(FGlobalTags::Get().Status_Invincible);

		if (!GetWorldTimerManager().IsTimerActive(InvincibilityHandle))
		{
			GetWorldTimerManager().SetTimer(
				InvincibilityHandle, this, &APlayerCharacter::RemoveInvincibility, DashInvincibilityDuration);
		}

		if (!this->bHasDashed)
		{
			this->bHasDashed = true;

			GetWorldTimerManager().SetTimer(DashHandle, this, &APlayerCharacter::ResetDash, DashCooldown, false);
		}
	}
}

void APlayerCharacter::ResetDash()
{
	this->bHasDashed = false;
}
// ---------------------------------------------------------------------------------------------

void APlayerCharacter::Attack()
{
	if (this->Gun != nullptr)
	{
		if (this->Gun->HeldAmmo)
		{
			// Play appropriate gun sound.
			switch (this->Gun->HeldAmmo->AmmoFireStyle)
			{
				case EFiringStyle::Burst:
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->GunSounds[0], GetActorLocation());
					break;
			}

			this->Gun->PullTrigger();

			if (!GetWorldTimerManager().IsTimerActive(ShootingHandle))
			{
				GetWorldTimerManager().SetTimer(
					ShootingHandle, this, &APlayerCharacter::Attack, 1 / this->Gun->HeldAmmo->ShotsPerSecond, true);
			}

			this->Gun->bIsFiring = true;
		}
	}
}

void APlayerCharacter::StopAttacking()
{
	if (GetWorldTimerManager().IsTimerActive(ShootingHandle)) { GetWorldTimerManager().ClearTimer(ShootingHandle); }

	if (this->Gun != nullptr) { this->Gun->bIsFiring = false; }
}

void APlayerCharacter::ReloadWeapon()
{
	if (this->Gun && this->ReloadAnimation && this->bAllowedReload && this->Gun->CanReload())
	{
		this->GetMesh()->GetAnimInstance()->Montage_Play(this->ReloadAnimation, this->Gun->ReloadTime);
	}
}

// -------------------------Ammo Equipping & Weapon Swap-------------------------
void APlayerCharacter::SwapWeapons()
{
	if (this->bAllowedAmmoEquip)
	{
		auto* PrimaryWeapon = this->PlayerInventory->CurrentItems[this->PlayerInventory->WeaponSlotIndex];
		auto* SecondaryWeapon = this->PlayerInventory->CurrentItems[this->PlayerInventory->SecondaryWeaponSlotIndex];

		// Whatever the case for the swap we need to nullify this guns'ammo.
		if (this->Gun != nullptr) { this->Gun->HeldAmmo = nullptr; }

		this->OnAmmoUpdated.Broadcast();

		if (PrimaryWeapon != nullptr)
		{
			// Both weapons exist so we can just perform a swap.
			if (SecondaryWeapon != nullptr)
			{
				this->PlayerInventory->SwapItems(SecondaryWeapon, this->PlayerInventory->WeaponSlotIndex);
			}
			// Otherwise we tehnically just unequip the current weapon.
			else
			{
				this->PlayerInventory->MoveItemToEmptySlot(
					PrimaryWeapon, this->PlayerInventory->SecondaryWeaponSlotIndex);
			}
		}
		// Only other possibility is that we don't have a primary weapon but do have a secondary one.
		else if (SecondaryWeapon != nullptr)
		{
			this->PlayerInventory->MoveItemToEmptySlot(SecondaryWeapon, this->PlayerInventory->WeaponSlotIndex);
		}
	}
}

void APlayerCharacter::EquipAmmo(EFiringStyle AmmoFireStyle)
{
	// Don't do anything if we already equipped this ammo type.
	if (this->Gun->HeldAmmo != nullptr && this->Gun->HeldAmmo->AmmoFireStyle == AmmoFireStyle) { return; }

	bool CanAcceptAmmo = false;

	for (EFiringStyle AcceptedAmmo : this->Gun->AcceptableAmmoTypes)
	{
		if (AmmoFireStyle == AcceptedAmmo)
		{
			CanAcceptAmmo = true;
			break;
		}
	}

	if (!CanAcceptAmmo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun doesn't accept this ammo type."));
		return;
	}

	switch (AmmoFireStyle)
	{
		case EFiringStyle::Standard:
			this->Gun->HeldAmmo = this->PlayerInventory->AmmoInventory[0];
			this->Gun->ReloadTime = 1.25;
			break;
		case EFiringStyle::Burst:
			this->Gun->HeldAmmo = this->PlayerInventory->AmmoInventory[1];
			this->Gun->ReloadTime = 1.75;
			break;
		case EFiringStyle::Shotgun:
			this->Gun->HeldAmmo = this->PlayerInventory->AmmoInventory[2];
			this->Gun->ReloadTime = 1.75;
			break;
		case EFiringStyle::Sniper:
			this->Gun->HeldAmmo = this->PlayerInventory->AmmoInventory[3];
			this->Gun->ReloadTime = 2;
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid FiringStyle enum passed."));
			break;
	}

	if (this->Gun->HeldAmmo != nullptr)
	{
		this->Gun->UpdateAmmo();
		this->OnAmmoUpdated.Broadcast();
	}
}

void APlayerCharacter::EquipStandardAmmo()
{
	if (this->Gun != nullptr && this->bAllowedAmmoEquip) { this->EquipAmmo(EFiringStyle::Standard); }
}

void APlayerCharacter::EquipBurstAmmo()
{
	if (this->Gun != nullptr && this->bAllowedAmmoEquip) { this->EquipAmmo(EFiringStyle::Burst); }
}

void APlayerCharacter::EquipShotgunAmmo()
{
	if (this->Gun != nullptr && this->bAllowedAmmoEquip) { this->EquipAmmo(EFiringStyle::Shotgun); }
}

void APlayerCharacter::EquipSniperAmmo()
{
	if (this->Gun != nullptr && this->bAllowedAmmoEquip) { this->EquipAmmo(EFiringStyle::Sniper); }
}
// ---------------------------------------------------------------------------------------------

void APlayerCharacter::ThrowGrenade()
{
	if (this->GrenadeClass != nullptr && this->GrenadeCount != 0)
	{
		FVector GrenadeSpawnLocation = GetMesh()->GetSocketLocation("GunAttachPoint");

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		this->Grenade = GetWorld()->SpawnActor<AGrenade>(
			this->GrenadeClass, GrenadeSpawnLocation, FRotator(0, 0, 0), SpawnParameters);

		this->Grenade->GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

		this->GrenadeCount--;
	}
}

void APlayerCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractiveObject* EnteringInteractable = Cast<IInteractiveObject>(OtherActor);

	// Nothing changes if the entering actors cast is NULL or if we cannot interact anymore with it.
	if (EnteringInteractable == nullptr
		|| (EnteringInteractable->bWasInteractedWith && !EnteringInteractable->bCanBeUsedAgain))
	{
		return;
	}

	if (this->CurrentInteractable != nullptr)
	{
		float PreviousDistance =
			FVector::Dist(this->GetActorLocation(), this->CurrentInteractable->InteractiveObjectLocation);
		float CurrentDistance = FVector::Dist(this->GetActorLocation(), OtherActor->GetActorLocation());

		// If we already have a focused interactable and this check
		// passes, we want to turn of the widget for the current one
		// and replace it with the newly entered interactable.
		if (CurrentDistance < PreviousDistance)
		{
			AActor* CurrentActor = Cast<AActor>(this->CurrentInteractable);
			this->CurrentInteractable->InteractionWidgetVisibility(
				CurrentActor->FindComponentByClass<class UWidgetComponent>(), false);

			this->CurrentInteractable = EnteringInteractable;
			this->CurrentInteractable->InteractionWidgetVisibility(
				OtherActor->FindComponentByClass<class UWidgetComponent>(), true);
		}
	}
	// No current interactable means that the
	// new one gets automatically assigned.
	else
	{
		this->CurrentInteractable = EnteringInteractable;
		this->CurrentInteractable->InteractionWidgetVisibility(
			OtherActor->FindComponentByClass<class UWidgetComponent>(), true);
	}
}

void APlayerCharacter::OnBoxEndOverlap(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractiveObject* LeavingInteractable = Cast<IInteractiveObject>(OtherActor);

	if (LeavingInteractable == this->CurrentInteractable)
	{
		this->CurrentInteractable->InteractionWidgetVisibility(
			OtherActor->FindComponentByClass<class UWidgetComponent>(), false);

		this->CurrentInteractable = nullptr;

		this->InteractionZone->GetOverlappingActors(OverlappingInteractables);

		FHitResult DummyHitResult;

		for (auto* Actor : OverlappingInteractables)
		{
			this->OnBoxBeginOverlap(nullptr, Actor, nullptr, 0, false, DummyHitResult);
		}
	}
}

void APlayerCharacter::OnInteract()
{
	if (this->CurrentInteractable != nullptr)
	{
		this->CurrentInteractable->InteractPure();

		if (!this->CurrentInteractable->bCanBeUsedAgain)
		{
			AActor* InteractableActor = Cast<AActor>(this->CurrentInteractable);

			this->OnBoxEndOverlap(nullptr, InteractableActor, nullptr, 0);
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("No interactable in focus.")); }
}

void APlayerCharacter::EquipWeapon()
{
	int32 WeaponIndex = this->PlayerInventory->WeaponSlotIndex;

	if (this->PlayerInventory->CurrentItems[WeaponIndex] == nullptr) { return; }

	auto* WeaponClass = Cast<UBaseWeapon>(this->PlayerInventory->CurrentItems[WeaponIndex]);

	if (WeaponClass->BlueprintGunClass)
	{
		FActorSpawnParameters SpawnParameters;

		SpawnParameters.Owner = this;

		this->Gun = GetWorld()->SpawnActor<ABaseGun>(WeaponClass->BlueprintGunClass, SpawnParameters);
		this->Gun->AttachToComponent(
			GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunAttachPoint"));
	}
}

void APlayerCharacter::UnequipWeapon()
{
	if (this->Gun != nullptr)
	{
		this->Gun->Destroy();
		this->Gun = nullptr;
	}
}

void APlayerCharacter::AddXP(float& ExperienceToAdd)
{
	this->CurrentXP += ExperienceToAdd;

	while (this->CurrentXP >= this->XPForNextLevel)
	{
		this->CurrentXP -= this->XPForNextLevel;

		this->PlayerLevelUp();
	}
}

void APlayerCharacter::PlayerLevelUp()
{
	this->CurrentLevel++;

	this->AvailableSkillPoints++;

	// Replenish HP & Stamina on each level up.
	this->PlayerStats->CurrentHealth = this->PlayerStats->MaximumHealth;
	this->PlayerStats->CurrentStamina = this->PlayerStats->MaximumStamina;

	this->XPForNextLevel = ((4 * (this->CurrentLevel ^ 3) + 100) / 5);

	// Spawn particle effect for level up.

	if (this->OnLevelUp.IsBound()) { this->OnLevelUp.Broadcast(); }
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(0);
}

void APlayerCharacter::RemoveInvincibility()
{
	GameplayTags.RemoveTag(FGlobalTags::Get().Status_Invincible);
}

void APlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}