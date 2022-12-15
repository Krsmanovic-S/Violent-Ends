#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponSystem : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API IWeaponSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	* Does the target have a ranged weapon equipped
	* @return True if the target has a ranged weapon equipped
	*/
	virtual bool HasRangedWeapon() const = 0;

	/**
	* Does the target have a melee weapon equipped
	* @return True if the target has a melee weapon equipped
	*/
	virtual bool HasMeleeWeapon() const = 0;

	/**
	* Try to attack using the ranged weapon
	* @return true if the attack successful, false if the target doesn't have a ranged weapon or attack failed
	*/
	virtual bool TryAttackRanged() = 0;

	/**
	* Try to attack using the melee weapon
	* @return true if the attack successful, false if the target doesn't have a melee weapon or attack failed
	*/
	virtual bool TryAttackMelee() = 0;
};
