#pragma once

#include "CoreMinimal.h"
#include "BaseCustomDamageType.h"
#include "FireDamageType.generated.h"


UCLASS()
class VIOLENTENDS_API UFireDamageType : public UBaseCustomDamageType
{
	GENERATED_BODY()

public:

	virtual float ReturnDamageAmount(class UEntityStats* EntityStats, float Damage) override;
};
