#pragma once

#include "CoreMinimal.h"
#include "WeaponFireMode.h"
#include "ViolentEnds/Equipment/EquipmentBase.h"

#include "WeaponBase.generated.h"

class UAmmoBase;

UCLASS()
class VIOLENTENDS_API UWeaponBase: public UEquipmentBase {

	GENERATED_BODY()

public:
	UWeaponBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon|FireMode")
	EWeaponFireMode WeaponFireMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float FireRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	int32 MagazineSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	int32 Reserve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float MaxDamageRange;



};
