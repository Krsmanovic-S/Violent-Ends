#include "ClearBlackboardKeyValue.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UClearBlackboardKeyValue::UClearBlackboardKeyValue()
{
    NodeName = TEXT("Clear Blackboard Key");
}

EBTNodeResult::Type UClearBlackboardKeyValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(BlackboardKey.SelectedKeyName);

    return EBTNodeResult::Succeeded;
}

FString UClearBlackboardKeyValue::GetStaticDescription() const
{
    return FString::Printf(TEXT("Clear: %s"), *BlackboardKey.SelectedKeyName.ToString());
}