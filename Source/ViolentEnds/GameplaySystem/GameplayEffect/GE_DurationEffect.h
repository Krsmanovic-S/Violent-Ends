#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_DurationEffect.generated.h"

/**
* Template class for dynamically created duration effect
*/
UCLASS()
class VIOLENTENDS_API UGE_DurationEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	
	UGE_DurationEffect();
};