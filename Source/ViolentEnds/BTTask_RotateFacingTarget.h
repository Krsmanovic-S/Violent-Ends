// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RotateFacingTarget.generated.h"

/**
 * 
 */
class UBehaviorTree;

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
