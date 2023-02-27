#pragma once

#include "CoreMinimal.h"
#include "HazardTypes.h"
#include "EnvironmentalHazardBase.generated.h"

UCLASS(Abstract)
class VIOLENTENDS_API AEnvironmentalHazardBase : public AActor
{
	GENERATED_BODY()

public:
	AEnvironmentalHazardBase();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Hazard")
	EHazardType HazardType;

	/**
	 * Function used to activate the effect
	 * Usually automatically called when a certain condition is met
	 * Can be called from other objects (for example chain explosion)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hazard")
	virtual void Activate();
	
	/**
	* Returns if this hazard object can be reused
	*/
	UFUNCTION(BlueprintPure, Category = "Hazard")
	virtual bool CanBeReused();

	/**
	* Blueprint event override 
	* Called when this object is activated
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Hazard")
	void OnActivated();

	/**
	* Blueprint implementation of OnActivated
	*/
	virtual void OnActivated_Implementation();
};