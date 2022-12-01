#include "DistanceCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UDistanceCheck::UDistanceCheck()
{
    NodeName = "Distance Check";
    bCreateNodeInstance = true;

    // Accept only objects for the blackboard key.
    BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

bool UDistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    AAIController* Controller = OwnerComp.GetAIOwner();

    if(!Controller || !Blackboard)
    {
        return false;
    }

    AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));

    if(!TargetActor)
    {
        return false;
    }

    // Controlled pawn is far enough away compared to our distance parameter.
    if(Controller->GetPawn()->GetDistanceTo(TargetActor) >= this->DistanceToCheck)
    {
        return false;
    }

    return true;
}