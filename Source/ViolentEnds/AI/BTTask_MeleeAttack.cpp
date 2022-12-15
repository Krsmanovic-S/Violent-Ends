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

	EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	// Get the AI pawn
	APawn* AIActor = OwnerComp.GetAIOwner()->GetPawn();

	// Try to use melee attack
	// return successful if the pawn successfully use a melee attack
	if (auto WeaponSystem = Cast<IWeaponSystem>(AIActor)) { 
		bool Success = WeaponSystem->TryAttackMelee();
		if (Success) NodeResult = EBTNodeResult::Succeeded;
	}

	return NodeResult;
}
