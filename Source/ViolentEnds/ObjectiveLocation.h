#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseQuest.h"
#include "ObjectiveLocation.generated.h"

UCLASS()
class VIOLENTENDS_API AObjectiveLocation : public AActor
{
	GENERATED_BODY()
	
public:	

	AObjectiveLocation();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* OverlapZone;

	UBaseQuest* RelevantQuest;

	uint8 LocationObjectiveIndex = 0;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
