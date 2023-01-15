#include "PlayerCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ViolentEnds/Inventory/CharacterInventoryComponent.h"
#include "ViolentEnds/Quest/Component/QuestComponent.h"

APlayerCharacterBase::APlayerCharacterBase()
{
	InventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->InitInventoryComponent(CharacterASC);

	InteractionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionDetection"));
	InteractionDetection->SetupAttachment(GetRootComponent());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-65.f, 0.f, 0.f));
	SpringArmComponent->TargetArmLength = 1100.f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetComponentTickEnabled(false);
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
	bIsRotating = false;
}

void APlayerCharacterBase::StartRotation()
{
	float LoopDuration = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(
		RotationTimer, this, &APlayerCharacterBase::CharacterRotationCallback, LoopDuration, true);
	bIsRotating = true;
}

void APlayerCharacterBase::EndRotation()
{
	GetWorld()->GetTimerManager().ClearTimer(RotationTimer);
	bIsRotating = false;
}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
	PlayerController = Cast<APlayerController>(NewController);
	if (PlayerController) { PlayerController->SetControlRotation(FRotator(-65.f, 0.f, 0.f)); }
}

void APlayerCharacterBase::CharacterRotationCallback()
{
	if (!PlayerController) { return; }

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	FVector HitLocation = HitResult.Location;
	FVector NewForward = HitLocation - GetActorLocation();
	NewForward.GetSafeNormal2D();

	FRotator NewRotation = FRotationMatrix::MakeFromXZ(NewForward, FVector::UpVector).Rotator();
	FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, GetWorld()->DeltaTimeSeconds, 0.25f);
	if (PlayerController) { PlayerController->SetControlRotation(NewRotation); }
}
