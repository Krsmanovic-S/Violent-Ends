#include "ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffect.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PierceCount = 0;
	BounceCount = 0;

	// Create the collision detection component and setup
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnProjectileOverlap);
	SetRootComponent(CollisionComponent);

	// Create Mesh component
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(GetRootComponent());

	// Create the movement component
	ProjectileMovementComponent =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectileBase::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// Define hit impact cue(s)
	FHitResult HitResult = FindHitImpact();
	FGameplayCueParameters CueParams;
	CueParams.Location = HitResult.Location;
	CueParams.Normal = HitResult.Normal;

	// Define the hit event
	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(TEXT(""));
	FGameplayEventData Payload;
	Payload.Target = OtherActor;
	Payload.Instigator = GetOwner();
	if (OtherActor == Owner) { return; }

	// Handle collision logic if the target is a character (target has ability system component)
	if (HitGameplayEffect.IsValid())
	{
		if (IAbilitySystemInterface* ABSI = Cast<IAbilitySystemInterface>(OtherActor))
		{
			if (auto ASC = ABSI->GetAbilitySystemComponent())
			{
				// Hits the character
				// Require tags
				ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("")));

				// Apply hit effect

				if (HitGameplayEffect.IsValid() && OwnerAbilitySystemComponent)
				{
					OwnerAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*HitGameplayEffect.Data.Get(), ASC);
				}
			}

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, EventTag, Payload);
		}
	}

	// The collision target is dynamic gameplay elements (emplacements, etc.)
	// Useful if the target has specific reaction to the projectile (destroy on impact, etc. )
	else if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(OtherActor)) {}

	// Handle collision logic if the target is static target (Walls/ Pillars, etc.)
	else {}

	if (OwnerAbilitySystemComponent) { OwnerAbilitySystemComponent->ExecuteGameplayCue(ImpactCueTag, CueParams); }
}

UAbilitySystemComponent* AProjectileBase::GetAbilitySystemComponent() const
{
	UAbilitySystemComponent* RetVal = nullptr;
	if (OwnerAbilitySystemComponent) { RetVal = OwnerAbilitySystemComponent; }
	return RetVal;
}

FHitResult AProjectileBase::FindHitImpact()
{
	FHitResult HitResult;

	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart + (GetActorForwardVector() * (CollisionComponent->GetScaledSphereRadius() + 5.f));
	ECollisionChannel TraceChannel = (CollisionComponent->GetCollisionObjectType());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel);

	return HitResult;
}
