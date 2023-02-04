#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemActorInterface.generated.h"

class UItemBase;

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UItemActorInterface : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API IItemActorInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual UItemBase* GetItem() const = 0;
};