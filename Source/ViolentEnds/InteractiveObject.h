#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractiveObject : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API IInteractiveObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Blueprint version of the interaction.
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void Interact();

	// C++ implementation of the interaction.
	UFUNCTION()
	virtual void InteractPure() = 0;

	UFUNCTION()
	virtual void InteractionWidgetVisibility(class UWidgetComponent* Widget, bool bShowWidget);

public:

	class UBaseQuest* RelevantQuest;

	int32 InteractiveObjectiveIndex;

	FVector InteractiveObjectLocation;

	bool bEnableWidgetSettings = true;

	bool bWasInteractedWith = false;

	bool bCanBeUsedAgain = false;

};
