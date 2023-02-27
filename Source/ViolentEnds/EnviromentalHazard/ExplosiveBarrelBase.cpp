#include "ExplosiveBarrelBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "GameplayEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "ViolentEnds/Subsystems/VE_WorldSubSystem.h"

AExplosiveBarrelBase::AExplosiveBarrelBase()
{
	BaseCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseCollision"));
	BaseCollision->SetSimulatePhysics(true);
	SetRootComponent(BaseCollision);

	ActivationDetection = CreateDefaultSubobject<USphereComponent>(TEXT("ActivationDetection"));
	ActivationDetection->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBarrelBase::OnActivationDetected);
	ActivationDetection->SetupAttachment(GetRootComponent());

	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	BarrelMeshComponent->SetupAttachment(GetRootComponent());

	ExplosionAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSFX"));
	ExplosionAudioComponent->SetupAttachment(GetRootComponent());

	VisualEffectSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VisualEffectSpawnLocation"));
	VisualEffectSpawnLocation->SetupAttachment(GetRootComponent());

	HazardType = EHazardType::Damaged;
}

void AExplosiveBarrelBase::OnActivationDetected_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Activate();
}

void AExplosiveBarrelBase::Activate()
{
	OnActivated();
}

bool AExplosiveBarrelBase::CanBeReused()
{
	return false;
}

void AExplosiveBarrelBase::OnActivated_Implementation()
{
	// Play vfx

	if (ExplosionVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(ExplosionVFX,
			VisualEffectSpawnLocation,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::Type::KeepRelativeOffset,
			true);
	}

	// Play sfx
	ExplosionAudioComponent->Play();

	// Damage calculation 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(
		ECollisionChannel::ECC_Pawn) };
	TArray<AActor*> IgnoreActors = { this };
	TArray<AActor*> OutActors;

	// Attempt to get all actors in the exposion radius
	bool Success = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), DamageRange, ObjectTypes, AActor::StaticClass(), IgnoreActors, OutActors);
	if (!Success) { return; }

	// Attempt to get the world's ASC to use to apply effect to the taget
	UVE_WorldSubsystem* AbilityWorldSubsystem = GetWorld()->GetSubsystem<UVE_WorldSubsystem>();
	if (!AbilityWorldSubsystem) return;

	// Attempt to apply explosion effect to all valid actors inside explosion radius
	for (int32 i = 0; i < OutActors.Num(); i++)
	{
		AbilityWorldSubsystem->TryApplyEffectToTargetActor(ExplosionEffect, OutActors[i]);
	}
}
