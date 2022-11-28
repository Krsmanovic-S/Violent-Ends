#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindRandomLocation.generated.h"


// BTT for finding random location on a nav mesh in a radius

UCLASS(Blueprintable)
class VIOLENTENDS_API UFindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UFindRandomLocation();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roaming Radius", meta = (AllowPrivateAccess = true));
	float SearchRadius = 1000.0f;
};
