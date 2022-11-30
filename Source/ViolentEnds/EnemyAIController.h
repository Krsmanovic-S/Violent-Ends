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


UCLASS()
class VIOLENTENDS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	virtual void OnUnPossess() override;

protected:

	virtual void OnPossess(APawn* InPawn) override; 

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors);

public:

	// Controller needs this for the FindRandomLocation task.
	UPROPERTY(BlueprintReadWrite)
	FVector EnemyOrigin;

	UPROPERTY(BlueprintReadWrite)
	bool bPlayerIsInView = false;

private:

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComp;

	ACharacter* PlayerCharacter;

	ABaseEnemy* ControlledEnemy;

};
