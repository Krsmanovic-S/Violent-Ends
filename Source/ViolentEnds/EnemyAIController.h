#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "EnemyAIController.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
class UBlackboardComponent;
class ABaseEnemy;
class UAIPerceptionComponent;
class APlayerCharacter;

UCLASS()
class VIOLENTENDS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	/* For now just calls the destroy method for this controller */
	virtual void OnUnPossess() override;

protected:
	/* Initializes the controlled enemy, its origin
	   and runs the selected behavior tree
	*/
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	/**
	 * Called whenever something is updated for the perception component,
	   for now only has functionality for responding to the Player.
	   Sets the appropriate blackboard keys if the Player is sensed and
	   clears them if the Player just left the perception range.

	 * @param UpdatedActors actors that have interacted with the perception component  
	 */
	UFUNCTION(BlueprintCallable)
	void OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors);

public:
	/* Starting point for the controlled pawn, used with the random
	   patrolling to keep it always within a certain distance from this value
	*/
	UPROPERTY(BlueprintReadWrite)
	FVector EnemyOrigin;

	/* This is used inside the Strafing EQS to make sure that
	   Enemies don't pick locations too close to each other
	*/
	TArray<AActor*> AllEnemies;

	APlayerCharacter* PlayerCharacter;

	ABaseEnemy* ControlledEnemy;

private:
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComp;
};
