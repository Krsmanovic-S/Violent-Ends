#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_RotateFacingTarget.generated.h"

class UBehaviorTree;

/**
* A simple task use to set the control rotation of the pawn to the target selected by the key
* Used if the pawn has character movement component with bUseControllerDesiredRotation (optionally bOrientRotationToMovement)
*/
UCLASS()
class VIOLENTENDS_API UBTTask_RotateFacingTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_RotateFacingTarget();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
