// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetInRangeCheck.generated.h"

/**
 * 
 */
class UBehaviorTree;

UCLASS()
class VIOLENTENDS_API UBTDecorator_TargetInRangeCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TargetInRangeCheck();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector TargetActorKey;	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector AttackRangeKey;

private:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
