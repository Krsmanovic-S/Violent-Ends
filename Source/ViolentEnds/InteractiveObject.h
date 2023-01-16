#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "InteractiveObject.generated.h"

class UWidgetComponent;
class UBaseQuest;

UENUM()
enum class InteractableType : uint8
{
	None,
	Chest,
	Door
};

UINTERFACE(MinimalAPI)
class UInteractiveObject : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API IInteractiveObject
{
	GENERATED_BODY()

	friend class UBaseQuest;

public:
	/* Blueprint version of the interaction */
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void Interact();

	/* C++ implementation of the interaction */
	UFUNCTION()
	virtual void InteractPure() = 0;

	/* Sets the visibility of the input widget
	   depending on the input boolean */
	UFUNCTION()
	virtual void InteractionWidgetVisibility(UWidgetComponent* Widget, bool bShowWidget);

public:

	/**
	 * Type determines what info message shows up if 
	 * the player cannot interact with this object.
	 */
	InteractableType Type;

	/* Quest that is connected to this interactive object */
	UBaseQuest* RelevantQuest;

	/* Index of the objective this object corresponds to,
	   the index is for the Objectives array of the quest
	*/
	int32 InteractiveObjectiveIndex;

	/* Location used for the quest widgets and trackers to
	   point to. Set by the quest as well if it was specified
	*/
	FVector InteractiveObjectLocation;

	bool bEnableWidgetSettings = true;

	/* Can the Player interact with this object now? */
	bool bCanInteract = true;

	/* Has the Player interacted with this object before? */
	bool bWasInteractedWith = false;

	/* If the Player interacted with this object, can he do so again? */
	bool bCanBeUsedAgain = false;
};
