#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


UCLASS()
class VIOLENTENDS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

protected:

	virtual void OnPossess(APawn* InPawn) override; 

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(BlueprintReadWrite)
	FVector EnemyOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerIsInView = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;

private:

	ACharacter* PlayerCharacter;

	class ABaseEnemy* ControlledEnemy;

	UPROPERTY(VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	class UBehaviorTreeComponent* BehaviorTreeComp;

};
