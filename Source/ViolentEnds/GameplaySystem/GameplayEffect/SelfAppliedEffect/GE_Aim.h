#pragma once

#include "CoreMinimal.h"
#include "ViolentEnds/GameplaySystem/GameplayEffect/GE_InstantEffect.h"
#include "GE_Aim.generated.h"

// Effect applied to self when the character uses aim
UCLASS()
class VIOLENTENDS_API UGE_Aim: public UGE_InstantEffect {

	GENERATED_BODY()

public:

	UGE_Aim();
};