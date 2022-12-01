#include "AttackPlayer.h"
#include "EnemyAIController.h"
#include "BaseEnemy.h"


UAttackPlayer::UAttackPlayer()
{
    NodeName = TEXT("Attack The Player");
}

EBTNodeResult::Type UAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABaseEnemy* EnemyPawn = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());

    if(EnemyPawn)
    {
        EnemyPawn->Attack();
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
