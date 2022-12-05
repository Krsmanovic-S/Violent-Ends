#include "EnvironmentalDamageActor.h"
#include "Components/BoxComponent.h"
#include "BaseCustomDamageType.h"
#include "EntityStats.h"
#include "TimerManager.h"


AEnvironmentalDamageActor::AEnvironmentalDamageActor()
{
	PrimaryActorTick.bCanEverTick = false;

	this->OverlapZone = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapZone"));
	RootComponent = this->OverlapZone;
}

void AEnvironmentalDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
	// For damage over time type actors this is what they will use,
	// for other types (i.e. frozen floor) the implementation will
	// be in blueprints (OnComponentBeginOverlap).
	if(this->EnvironmentalDamageType != nullptr)
	{
		GetWorldTimerManager().SetTimer(this->DamageHandle, this, &AEnvironmentalDamageActor::ApplyDamageOverTime,
										this->TimeToDealDamage, true);
	}

	// If lifetime was specified then set it up.
	if(this->LifeTime != 0)
	{
		this->SetLifeSpan(this->LifeTime);
	}
}

void AEnvironmentalDamageActor::ApplyDamageOverTime()
{
	this->OverlapZone->GetOverlappingActors(OverlappingActors);

	FDamageEvent DamageEvent;
	float CalculatedDamage = 1;

	for(AActor* Actor : this->OverlappingActors)
	{
		if(Actor->CanBeDamaged())
		{
			if(Actor->FindComponentByClass<UEntityStats>())
			{
				CalculatedDamage = this->EnvironmentalDamageType.GetDefaultObject()->ReturnDamageAmount(Actor->FindComponentByClass<UEntityStats>(), this->DamageToDeal);
			}
			Actor->TakeDamage(CalculatedDamage, DamageEvent, NULL, this);
		}
	}
}

