// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseMeleeWeapon.h"

#include "BaseCustomDamageType.h"
#include "BaseEnemy.h"
#include "BaseItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.h"

// EDITOR only headers

// #if WITH_EDITOR
#include "DrawDebugHelpers.h"
// #endif

// Sets default values
ABaseMeleeWeapon::ABaseMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bCanUseMelee = true;

	MeleeWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeleeWeaponMesh"));

	SetRootComponent(MeleeWeaponMesh);

	// #if WITH_EDITOR
	bShowMeleeDebug = true;
	// #endif
}

void ABaseMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	AActor* WeaponOwner = GetOwner();
	if (auto PlayerCharacter = Cast<APlayerCharacter>(WeaponOwner))
	{
		int32 WeaponIndex = PlayerCharacter->PlayerInventory->WeaponSlotIndex;
		auto* WeaponItem = PlayerCharacter->PlayerInventory->CurrentItems[WeaponIndex];

		WeaponDamageTypes = WeaponItem->ItemStats.ItemDamageTypes;
	}
}

bool ABaseMeleeWeapon::Attack()
{
	// The owner of this component
	AActor* WeaponOwner = GetOwner();

	// Check if the actor is alive or this component is attached to an actor
	if (!WeaponOwner || !bCanUseMelee) { return false; }

	//----------------------------
	//	DEBUG
	// #if WITH_EDITOR

	if (bShowMeleeDebug)
	{
		AActor* TransformRef = GetOwner() ? GetOwner() : this;
		DrawDebugCone(GetWorld(), TransformRef->GetActorLocation(), TransformRef->GetActorForwardVector(), MeleeRange,
			FMath::Cos(Angle), FMath::Cos(Angle), 12, FColor::Green, false, DebugDuration);
	}

	// #endif // WITH_EDITOR
	//----------------------------

	if (OnMeleeWeaponAttack.IsBound()) { OnMeleeWeaponAttack.Broadcast(); }

	bCanUseMelee = false;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(MeleeCooldown))
	{
		TimerManager.SetTimer(MeleeCooldown, this, &ABaseMeleeWeapon::AttackReset, MeleeCooldownDuration, false);
	}

	// Parameters for sphere overlap function
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_Pawn) }; // Object types
	UClass* TargetClass = AActor::StaticClass();	// The sphere overlap class check

	TArray<AActor*> IgnoreActors; // List of actors to ignore
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WeaponOwner->GetClass(), IgnoreActors); 
	
	TArray<AActor*> HitResults;						// Overlap result

	// Sphere overlap check to see if there are any applicable "enemies" nearby
	bool Success = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), Owner->GetActorLocation(), MeleeRange, ObjectTypes, TargetClass, IgnoreActors, HitResults);

	// Did not overlap with anything
	if (!Success) { return true; }

	// Convert the angle from degrees to dot product
	float MeleeDotAngle = FMath::Cos(Angle);

	// Get the owner vector properties
	FVector OwnerForwardDirection = Owner->GetActorForwardVector();
	FVector OwnerCurrentLocation = Owner->GetActorLocation();

	// The final array of actors to apply damage to
	TArray<AActor*> ActorInDamageRange;

	for (int32 i = 0; i < HitResults.Num(); i++)
	{
		FVector CurrentTargetLocation = HitResults[i]->GetActorLocation();
		FVector DirectionFromOwnerToTarget = (CurrentTargetLocation - OwnerCurrentLocation);
		DirectionFromOwnerToTarget.GetSafeNormal2D();

		float DotAngle = FVector::DotProduct(OwnerForwardDirection, DirectionFromOwnerToTarget);

		if (DotAngle >= MeleeDotAngle) { ActorInDamageRange.Add(HitResults[i]); }
	}

	for (int32 i = 0; i < ActorInDamageRange.Num(); i++)
	{
		DealDamage(ActorInDamageRange[i]);
	}

	return true;
}

void ABaseMeleeWeapon::AttackReset()
{
	bCanUseMelee = true;

	if (OnMeleeWeaponAttackCooldownCleared.IsBound()) { OnMeleeWeaponAttackCooldownCleared.Broadcast(); }

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(MeleeCooldown)) { TimerManager.ClearTimer(MeleeCooldown); }
}

void ABaseMeleeWeapon::DealDamage(AActor* Target)
{
	AActor* WeaponOwner = GetOwner();
	for (auto& Item : WeaponDamageTypes)
	{
		UGameplayStatics::ApplyDamage(Target, Item.Value, nullptr, WeaponOwner, Item.Key);
	}
}