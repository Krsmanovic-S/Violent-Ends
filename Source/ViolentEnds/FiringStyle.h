#pragma once

#include "CoreMinimal.h"
#include "FiringStyle.generated.h"


UENUM(BlueprintType)
enum class EFiringStyle : uint8
{ 
    Standard  UMETA(DisplayName = "Standard"),
    Burst     UMETA(DisplayName = "Burst"),
    Shotgun   UMETA(DisplayName = "Shotgun"),
    Sniper    UMETA(DisplayName = "Sniper")
};