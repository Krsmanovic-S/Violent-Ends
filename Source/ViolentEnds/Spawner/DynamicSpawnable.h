#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DynamicSpawnable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDynamicSpawnable : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API IDynamicSpawnable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	* Get the spawn offset of the actor
	* For characters, usually the half height of it's cylinder
	* @return The offset of the character
	*/
	virtual FVector GetSpawnLocationOffset() const = 0;

	/**
	* Should the actor be spawned facing a specific direction
	*/
	virtual bool ShouldSpawnWithFixedRotation();

	/**
	* The absolute rotation of the spawned actor if the actor
	*/
	virtual FRotator GetFixedSpawnRotation() const = 0;


	/**
	* Get the radius of the spawning actor
	*/
	virtual float GetSpawnRadius() const = 0;
};
