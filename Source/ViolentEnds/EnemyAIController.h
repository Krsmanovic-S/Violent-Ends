#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

#include "EnemyAIController.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
class UBlackboardComponent;
class ABaseEnemy;
class UAIPerceptionComponent;
class APlayerCharacter;

/**
 * Used imperatively in the Behavior tree to select which Tasks the Controller will follow based on state.
 */
UENUM(BlueprintType)
enum class EAIEnemyState : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	AlertedToLocation	UMETA(DisplayName = "AlertedToLocation"),
	LostVisualPursuit	UMETA(DisplayName = "LostVisualPursuit"),
	VisualPursuit		UMETA(DisplayName = "VisualPursuit"),
	Attacking			UMETA(DisplayName = "Attacking"),
	// Flanking			
	// Retreating,
};


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
	 * Called whenever something is updated for the perception component, 3 Types of sense are active on the EnemyAI
	 * Damage, Sight, and Sound. Damage and Sound make the AI interested in the cause location, Sight will override
	 * and the PlayerController will become a visual focus.

	 * @param UpdatedActors actors that have interacted with the perception component  
	 */
	UFUNCTION(BlueprintCallable)
	void OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors);

	// Convenience IDs for testing which Perception type was sensed
	const FAISenseID DamageID = UAISense::GetSenseID<UAISense_Damage>();
	const FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
	const FAISenseID HearID = UAISense::GetSenseID<UAISense_Hearing>();

public:
	/* Starting point for the controlled pawn, used with the random
	   patrolling to keep it always within a certain distance from this value
	*/
	UPROPERTY(BlueprintReadWrite, Category = "Defaults")
	FVector EnemyOrigin;

	/* This is used inside the Strafing EQS to make sure that
	   Enemies don't pick locations too close to each other
	*/
	// TArray<AActor*> AllEnemies; //Depricated

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter* PlayerCharacter;

	ABaseEnemy* ControlledEnemy;

	EAIEnemyState EnemyState; //Depricated

private:
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UFUNCTION()
	void InitAIPerceptionSenses();
};
