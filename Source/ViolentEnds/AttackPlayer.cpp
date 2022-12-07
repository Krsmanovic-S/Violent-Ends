#include "AttackPlayer.h"
#include "EnemyAIController.h"
#include "BaseEnemy.h"
#include "PlayerCharacter.h"
#include "Lib/BPFL_Math.h"
#include "GameFramework/CharacterMovementComponent.h"


UAttackPlayer::UAttackPlayer()
{
    NodeName = TEXT("Attack The Player");
}

EBTNodeResult::Type UAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

    if(EnemyController != nullptr && EnemyController->ControlledEnemy->bCanAttack)
    {
        if(EnemyController->ControlledEnemy->RangedWeapon != nullptr)
        {
            // Predict where to shoot before you do, the 9000 magic number
            // is the default projectile velocity from ABaseGun.
            FRotator PredictedRotation = UBPFL_Math::PredictRotationToMovingActor(
                EnemyController->ControlledEnemy->GetActorLocation(), 
                9000, 
                EnemyController->PlayerCharacter->GetActorLocation(), 
                EnemyController->PlayerCharacter->GetCharacterMovement()->GetLastUpdateVelocity()
            );

            EnemyController->ControlledEnemy->SetActorRotation(PredictedRotation);
        }
        EnemyController->ControlledEnemy->Attack();
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
