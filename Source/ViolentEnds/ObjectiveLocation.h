#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ObjectiveLocation.generated.h"

class UBoxComponent;
class UBaseQuest;

UCLASS()
class VIOLENTENDS_API AObjectiveLocation : public AActor
{
	GENERATED_BODY()

public:
	AObjectiveLocation();

	friend class UBaseQuest;

protected:
	virtual void BeginPlay() override;

	/* On player overlap, if the relevant quest and the relevant
	   objective is active, call the UpdateObjective method for it
	*/
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	/* Zone for the overlap trigger, triggers the member function OnBoxBeginOverlap */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* OverlapZone;

	/* What quest does this actor belong to? This is set by the quest class */
	UBaseQuest* RelevantQuest;

	/* Index of the objective this actor corresponds to.
	   Refers to the Objectives TArray from the quest class
	*/
	uint8 LocationObjectiveIndex = 0;
};
