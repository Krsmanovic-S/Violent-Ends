#pragma once

#include "CoreMinimal.h"
#include "QuestObjectiveTypes.generated.h"

UENUM(BlueprintType)
enum class EQuestObjectiveType: uint8 {
	Type UMETA(DisplayName = "Type", Tooltip = "Default value, can be considered as none or null"),
	Destination UMETA(DisplayName = "Destination", Tooltip = ""),
	Elimination UMETA(DisplayName = "Elimination", Tooltip = ""),

};
