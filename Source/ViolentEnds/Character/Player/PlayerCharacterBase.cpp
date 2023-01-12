#include "PlayerCharacterBase.h"

#include "Components/BoxComponent.h"
#include "ViolentEnds/Inventory/CharacterInventoryComponent.h"

APlayerCharacterBase::APlayerCharacterBase() {

	InventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("InventoryComponent"));
	InteractionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionDetection"));
	InteractionDetection->SetupAttachment(GetRootComponent());
	bIsRotating = false;
}

void APlayerCharacterBase::StartRotation() 
{
	float LoopDuration = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(RotationTimer,this, &APlayerCharacterBase::CharacterRotationCallback, LoopDuration, true);
	bIsRotating = true;
}

void APlayerCharacterBase::EndRotation() 
{
	GetWorld()->GetTimerManager().ClearTimer(RotationTimer);
	bIsRotating = false;
}

void APlayerCharacterBase::PossessedBy(AController* NewController) {
	PlayerController = Cast<APlayerController>(NewController); 
}

void APlayerCharacterBase::CharacterRotationCallback() {

	//if (!PlayerController) return;

	//FHitResult HitResult;
	//PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	//// The mouse hit result occurs at some arbitrary point, from the camera to the floor/ terrain
	//// Interpolate or extrapolate that trace to derive the point
	//// where the line's Z value is equal to the player's gun height (roughly, player location Z + 55)
	//// The player's location there is its half height, so it would be around 135cm above the current floor
	//FVector HitLocation = HitResult.Location;
	//FVector CameraLocation = HitResult.TraceStart;
	//FVector TraceDirection = (CameraLocation - HitLocation).GetSafeNormal();
	//float PlayerGunHeight = GetActorLocation().Z + GGun_Height;
	//float HitToPlayerGunDistance = PlayerGunHeight - HitLocation.Z;
	//FVector OffsetToLineIntersection = HitToPlayerGunDistance * (1 / TraceDirection.Z) * TraceDirection;

	//FVector Target = HitLocation + OffsetToLineIntersection;

	//RotateCharacterToMouse(Target, DeltaTime);
	//
	//else if (MainPlayerController && bIsRelativeAiming)
	//{
	//	FVector Target = GetActorLocation() + (RelativeAimDirection * 100);
	//	RotateCharacterToMouse(Target, DeltaTime);
	//}

	//if (this->PlayerStats->bIsEntityRunning && this->PlayerStats->CurrentStamina > 0)
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	//}
	//else { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; }
}

