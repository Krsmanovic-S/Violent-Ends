// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCustomDamageType.h"
#include "PhysicalDamageType.generated.h"


UCLASS()
class VIOLENTENDS_API UPhysicalDamageType : public UBaseCustomDamageType
{
	GENERATED_BODY()

public:
	
	virtual float ReturnDamageAmount(class UEntityStats* EntityStats, float Damage) override;
};
