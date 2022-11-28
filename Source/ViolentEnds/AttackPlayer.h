#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackPlayer.generated.h"


UCLASS()
class VIOLENTENDS_API UAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UAttackPlayer();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
