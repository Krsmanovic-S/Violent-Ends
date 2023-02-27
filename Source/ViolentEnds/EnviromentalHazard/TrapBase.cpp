#include "TrapBase.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Curves/CurveFloat.h"
#include "GameplayEffect.h"
#include "HazardTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "ViolentEnds/Subsystems/VE_WorldSubSystem.h"

ATrapBase::ATrapBase()
{
	TrapMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMeshComponent"));
	VisualEffectSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VisualEffectSpawnLocation"));
	ActivationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActivationAudioComponent"));

	TrapMeshComponent->SetupAttachment(GetRootComponent());
	VisualEffectSpawnLocation->SetupAttachment(GetRootComponent());
	ActivationAudioComponent->SetupAttachment(GetRootComponent());

	HazardType = EHazardType::Trap;
}

void ATrapBase::OnActivationDetected_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ATrapBase::Activate()
{
	OnActivated();
}

bool ATrapBase::CanBeReused()
{
	return false;
}

void ATrapBase::OnActivated_Implementation()
{
	if (ActivationVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(ActivationVFX,
			VisualEffectSpawnLocation,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::Type::KeepRelativeOffset,
			true);
	}

	ActivationAudioComponent->Play();
}

#if WITH_EDITOR

void ATrapBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ATrapBase, HazardShape))
	{
		ActivationDetection->OnComponentBeginOverlap.RemoveAll(this);
		ActivationDetection->DestroyComponent();
		switch (HazardShape)
		{
			case EHazardShape::Shape:
				break;
			case EHazardShape::Sphere:
				ActivationDetection = NewObject<USphereComponent>(this, TEXT("ActivationDetection"));
				break;
			case EHazardShape::Box:
				ActivationDetection = NewObject<UBoxComponent>(this, TEXT("ActivationDetection"));
				break;
			case EHazardShape::Capsule:
				ActivationDetection = NewObject<UCapsuleComponent>(this, TEXT("ActivationDetection"));
				break;
			default:
				break;
		}

		ActivationDetection->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnActivationDetected);
	}
}
#endif // WITH_EDITOR