#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ClearBlackboardKeyValue.generated.h"


// BT Task for clearing a specific blackboard key.
UCLASS()
class VIOLENTENDS_API UClearBlackboardKeyValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UClearBlackboardKeyValue();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
