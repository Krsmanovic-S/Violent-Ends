#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class UItemBase;

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API IInteractableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void InteractWithObject(AActor* Source = nullptr) = 0;
};