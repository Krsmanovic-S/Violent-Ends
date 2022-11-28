#include "ObjectiveLocation.h"
#include "Components/BoxComponent.h"


AObjectiveLocation::AObjectiveLocation()
{
	PrimaryActorTick.bCanEverTick = false;

	this->OverlapZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Zone"));
	RootComponent = this->OverlapZone;

	this->OverlapZone->SetGenerateOverlapEvents(true);
}

void AObjectiveLocation::BeginPlay()
{
	Super::BeginPlay();

	this->OverlapZone->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveLocation::OnBoxBeginOverlap);
}

void AObjectiveLocation::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(this->RelevantQuest != NULL && this->RelevantQuest->Objectives[this->LocationObjectiveIndex].bIsActive)
	{
		this->RelevantQuest->UpdateObjective(this->RelevantQuest->Objectives[this->LocationObjectiveIndex]);
	}
}

