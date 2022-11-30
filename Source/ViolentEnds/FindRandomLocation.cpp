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
    FNavLocation RandomLocation;

    // Get AI Controller
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

    // Obitain Navigation System to find a random location.
    const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    uint8 LoopIterations = 0;
    float Distance;

    if(NavSystem != NULL && NavSystem->GetRandomPointInNavigableRadius(AIController->EnemyOrigin, this->SearchRadius, RandomLocation))
    {
        do
        {
            // Find a random location in a circle, whose center is EnemyOrigin and radius is SearchRadius.
            NavSystem->GetRandomPointInNavigableRadius(AIController->EnemyOrigin, this->SearchRadius, RandomLocation);

            // We would prefer locations at least 100 units away.
            Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), RandomLocation.Location);

        } while (LoopIterations < 100 && Distance < 100);
    }

    if(Distance >= 100)
    {
         AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, RandomLocation.Location);

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