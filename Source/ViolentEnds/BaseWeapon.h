#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseWeapon.generated.h"

class ABaseGun;
class ABaseMeleeWeapon;

UCLASS()
class VIOLENTENDS_API UBaseWeapon : public UBaseItem
{
	GENERATED_BODY()
	
public:

	virtual void Use(class APlayerCharacter* Player) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABaseGun> BlueprintGunClass;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABaseMeleeWeapon> BlueprintMeleeClass;

	UPROPERTY(BlueprintReadWrite)
	class UBaseAmmo* ItemHeldAmmo;
};
