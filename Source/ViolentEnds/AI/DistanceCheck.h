#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "DistanceCheck.generated.h"


// Decorator for comparing whether the Owner is too close or too far away to another Actor.
UCLASS()
class VIOLENTENDS_API UDistanceCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UDistanceCheck();

private:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Condition")
	float DistanceToCheck = 200.f;
};
