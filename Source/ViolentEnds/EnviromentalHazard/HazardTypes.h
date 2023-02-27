#pragma once

#include "CoreMinimal.h"

#include "HazardTypes.generated.h"

UENUM(BlueprintType)
enum class EHazardType : uint8 {
	Type,
	Trap UMETA(ToolTip = "Activates when a character enters its activation range"),
	Damaged UMETA(ToolTip = "Activates when a projectile hit it"),
	Conditioned UMETA(ToolTip = "Activates when a certain condition is met (usually through tags)")
};

/**
* 
*/
UENUM(BlueprintType)
enum class EHazardShape : uint8
{
	Shape ,
	Sphere UMETA(ToolTip = ""),
	Box UMETA(ToolTip = ""),
	Capsule UMETA(ToolTip = "")
};