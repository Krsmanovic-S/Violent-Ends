#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BaseCustomDamageType.generated.h"


UCLASS()
class VIOLENTENDS_API UBaseCustomDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	virtual float ReturnDamageAmount(class UEntityStats* EntityStats, float Damage);

	UFUNCTION(BlueprintCallable)
	void CriticalHit(class UEntityStats* EntityStats, float& CurrentDamageValue);
};
