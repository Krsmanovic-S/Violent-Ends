#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BaseCustomDamageType.generated.h"


class UEntityStats;

UCLASS()
class VIOLENTENDS_API UBaseCustomDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	virtual float ReturnDamageAmount(UEntityStats* EntityStats, float Damage);

	UFUNCTION(BlueprintCallable)
	void CriticalHit(UEntityStats* EntityStats, float& CurrentDamageValue);
};
