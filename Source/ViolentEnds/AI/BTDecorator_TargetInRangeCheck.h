#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"

#include "BTDecorator_TargetInRangeCheck.generated.h"

class UBehaviorTree;

/**
* A simple decorator used to determined if the target is in range
*/
UCLASS()
class VIOLENTENDS_API UBTDecorator_TargetInRangeCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TargetInRangeCheck();

	/**
	* The key that is used as the target enemy actor
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	/**
	* The key that is used as the detection range
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector AttackRangeKey;

	/**
	* The multiplier which increases or decreases the detection range, default = 1.0
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RangeModifierMultiplier;

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
