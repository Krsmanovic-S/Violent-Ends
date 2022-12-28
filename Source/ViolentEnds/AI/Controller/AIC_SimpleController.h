// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIC_SimpleController.generated.h"

//struct FAIStimulus;
class UAISenseConfig_Sight;

/**
 * Class of a simple AI controller that includes detecting the player and update it's info to the blackboard
 * Also includes reading from the blackboard a value named "ResetTime" which will reset the blackboard player key to
 * null
 */
UCLASS()
class VIOLENTENDS_API AAIC_SimpleController : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_SimpleController();

	///	PROPERTIES

	/**
	 * The Perception component used to detect the actors
	 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	/**
	 * The range of the ocular detection radius
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Sight")
	float SightRange;

	/**
	 * The angle of sight detection
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Sight")
	float SightAngle;

	/**
	 * The behaviour tree used to run when the pawn is possesed
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	/**
	 * Timer used to reset detection
	 */
	FTimerHandle ResetPerceptionTimer;

	/**
	 * The sight sense of the AI
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	UAISenseConfig_Sight* AISight;

	///		FUNCTIONS

	/**
	 * Called when perception info is updated
	 */
	UFUNCTION()
	void OnActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/**
	 * Called when the controller posseses a new pawn
	 */
	virtual void OnPossess(APawn* InPawn) override;
};
