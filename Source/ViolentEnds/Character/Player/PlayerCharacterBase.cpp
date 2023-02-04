#include "PlayerCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ViolentEnds/Interactable/InteractableInterface.h"
#include "ViolentEnds/Inventory/CharacterInventoryComponent.h"
#include "ViolentEnds/Quest/Component/QuestComponent.h"

APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->InitInventoryComponent(CharacterASC);
	InventoryComponent->OnWeaponSheathed;
	InventoryComponent->OnWeaponSwapped;

	InteractionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionDetection"));
	InteractionDetection->SetupAttachment(GetRootComponent());
	InteractionDetection->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacterBase::OnOverlapBegin);
	InteractionDetection->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacterBase::OnOverlapEnd);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-65.f, 0.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;
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

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));

	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
	bIsAiming = false;
}


void APlayerCharacterBase::InteractWithItem()
{
	if (auto Interactable = Cast<IInteractableInterface>(CurrentInteractable))
	{
		Interactable->InteractWithObject(this);
	}
}

void APlayerCharacterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Interactable = Cast<IInteractableInterface>(OtherActor)) { InteractablesInRange.AddUnique(OtherActor); }
	InteractablesInRange.Sort([this](const AActor& A, const AActor& B) {
		return FVector::DistSquared(GetActorLocation(), A.GetActorLocation())
			   < FVector::DistSquared(GetActorLocation(), B.GetActorLocation());
	});

	HighlightInteraction();
}

void APlayerCharacterBase::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractablesInRange.Find(OtherActor) >= 0) { InteractablesInRange.Remove(OtherActor); }
	InteractablesInRange.Sort([this](const AActor& A, const AActor& B) {
		return FVector::DistSquared(GetActorLocation(), A.GetActorLocation())
			   < FVector::DistSquared(GetActorLocation(), B.GetActorLocation());
	});

	HighlightInteraction();
}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
	PlayerController = Cast<APlayerController>(NewController);
	if (PlayerController) { PlayerController->SetControlRotation(FRotator(-65.f, 0.f, 0.f)); }
}

void APlayerCharacterBase::Tick(float DeltaSecond)
{
	CharacterRotationCallback();
}

void APlayerCharacterBase::HighlightInteraction_Implementation() {
	
	if (InteractablesInRange.Num() > 0)
	{
		CurrentInteractable = InteractablesInRange[0];
		// Toggle highlight
		if (!InteractionWidget->IsActive()) { InteractionWidget->SetActive(true); }
		InteractionWidget->AttachToComponent(
			CurrentInteractable->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		if (InteractionWidget->IsActive()) { InteractionWidget->SetActive(false); }
		CurrentInteractable = nullptr;
	}
}

void APlayerCharacterBase::OnWeaponSheathed_Implementation(UWeaponBase* Weapon) {
	// Play animation

	// Block the character from being able to swap weapons until the animation is complete
	//GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic();
}

void APlayerCharacterBase::OnWeaponSwapped_Implementation(UWeaponBase* OldWeapon, UWeaponBase* NewWeapon) {
	// Play animation

	// Block the character from being able to swap weapons until the animation is complete
	//GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic();
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
