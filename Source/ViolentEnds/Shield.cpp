#include "Shield.h"

#include "BaseEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "EntityStats.h"
#include "PlayerCharacter.h"
#include "ViolentEnds/LogMacros.h"

AShield::AShield()
{
	PrimaryActorTick.bCanEverTick = false;

	this->ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(this->ShieldMesh);

	this->ShieldStats = CreateDefaultSubobject<UEntityStats>(TEXT("ShieldStats"));
}

void AShield::BeginPlay()
{
	Super::BeginPlay();

	if (this->ShieldOwner != nullptr)
	{
		if (this->ShieldOwner->IsA<APlayerCharacter>()) { this->bIgnorePlayerShield = true; }
		else { this->bIgnoreEnemyShield = true; }
	}
	else { LOG_WARNING(LogTemp, "Shield owner wasn't set on spawn."); }
}