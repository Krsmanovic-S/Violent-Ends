#pragma once

#include "CoreMinimal.h"
#include "ViolentEnds/GameplaySystem/GameplayEffect/GE_InfiniteEffect.h"
#include "ViolentEnds/Equipment/EquipmentType.h"
#include "GE_EquipmentEffect.generated.h"

/**/
UCLASS()
class VIOLENTENDS_API UGE_EquipmentEffect : public UGE_InfiniteEffect
{
	GENERATED_BODY()

public:

	UGE_EquipmentEffect();
	
	static UGE_EquipmentEffect* CreateEffect(FEquipmentEffectSpec EffectSpec);

};