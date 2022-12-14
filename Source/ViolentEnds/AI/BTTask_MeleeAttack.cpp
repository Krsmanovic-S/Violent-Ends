#include "BTTask_MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ViolentEnds/BaseMeleeWeapon.h"
#include "ViolentEnds/WeaponSystem.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = "MeleeAttack";
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	APawn* AIActor = OwnerComp.GetAIOwner()->GetPawn();
	if (auto WeaponSystem = Cast<IWeaponSystem>(AIActor)) { WeaponSystem->TryAttackMelee(); }

	return NodeResult;
}
