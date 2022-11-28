#include "FindRandomLocation.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UFindRandomLocation::UFindRandomLocation()
{
    NodeName = TEXT("Find Random Location");

    // Accept Only Vectors
    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(
                                  UFindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FNavLocation Location;

    // Get AI Controller
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

    // Obitain Navigation System and find a random location
    const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    if(NavSystem != NULL && NavSystem->GetRandomPointInNavigableRadius(AIController->EnemyOrigin, this->SearchRadius, Location))
    {
        AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);

        // Signal the BT Comp that task finished with success
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

FString UFindRandomLocation::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}