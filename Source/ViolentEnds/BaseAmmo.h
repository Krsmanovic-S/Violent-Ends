#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "FiringStyle.h"
#include "BaseAmmo.generated.h"


UCLASS()
class VIOLENTENDS_API UBaseAmmo : public UBaseItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFiringStyle AmmoFireStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShotsPerSecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AmmoMagazineSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AmountToFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GunFiringRange = 500.f;
};
