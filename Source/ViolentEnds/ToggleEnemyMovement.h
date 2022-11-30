#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ToggleEnemyMovement.generated.h"


// Toggles Walking/Running for Enemies.
UCLASS()
class VIOLENTENDS_API UToggleEnemyMovement : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UToggleEnemyMovement();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
