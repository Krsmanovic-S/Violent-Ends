#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_LineOfSight.generated.h"


UCLASS()
class VIOLENTENDS_API UEnvQueryTest_LineOfSight : public UEnvQueryTest
{
	GENERATED_BODY()

	UEnvQueryTest_LineOfSight(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TSubclassOf<UEnvQueryContext> Context;
	
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
