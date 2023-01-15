#pragma once

#include "CoreMinimal.h"
#include "QuestObjectiveTypes.h"
#include "GameplayTagContainer.h"

#include "QuestObjective.generated.h"

class UBoxComponent;

/**
* Single use quest objective, activates when the player start overlapping with the objective detection area (Box component)
* Will be called for destroy when successfully detect and apply quest tag to the player character ASC
*/
UCLASS()
class VIOLENTENDS_API AQuestObjectiveBase : public AActor
{
	GENERATED_BODY()

public:
	AQuestObjectiveBase();

	UPROPERTY()
	EQuestObjectiveType QuestType = EQuestObjectiveType::Type;

	UPROPERTY()
	FGameplayTagContainer QuestObjectiveTag;

	UPROPERTY()
	UBoxComponent* ObjectiveArea;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};