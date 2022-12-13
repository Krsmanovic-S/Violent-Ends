// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MoveDirectlyToTarget.generated.h"

/**
 * 
 */
class UBehaviorTree;

UCLASS()
class VIOLENTENDS_API UBTTask_MoveDirectlyToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MoveDirectlyToTarget();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector TargetLocationKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoveRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector RadiusKey;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
