#include "ToggleEnemyMovement.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"


UToggleEnemyMovement::UToggleEnemyMovement()
{
    NodeName = TEXT("Toggle Enemy Movement");
}

EBTNodeResult::Type UToggleEnemyMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());

    if(EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed == EnemyCharacter->WalkMovementSpeed)
    {
        EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyCharacter->RunMovementSpeed;
    }
    else
    {
        EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyCharacter->WalkMovementSpeed;
    }

    return EBTNodeResult::Succeeded;
}

FString UToggleEnemyMovement::GetStaticDescription() const
{
    return FString::Printf(TEXT("Toggle Walk/Run Speed"));
}