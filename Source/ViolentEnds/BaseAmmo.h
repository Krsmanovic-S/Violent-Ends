#pragma once

#include "CoreMinimal.h"

#include "BaseItem.h"

#include "BaseAmmo.generated.h"

/* Enum for the four different types of firing */
UENUM(BlueprintType)
enum class EFiringStyle : uint8
{
	Standard UMETA(DisplayName = "Standard"),
	Burst UMETA(DisplayName = "Burst"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Sniper UMETA(DisplayName = "Sniper")
};

UCLASS()
class VIOLENTENDS_API UBaseAmmo : public UBaseItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFiringStyle AmmoFireStyle;

	/* How often can we shoot this ammo? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShotsPerSecond;

	/* Size of the entire magazine when equipping this ammo */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AmmoMagazineSize;

	/* How many projectiles do we launch when we fire with this ammo? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AmountToFire;

	/* Range to which the projectiles will travel before disappearing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GunFiringRange = 500.f;

	/* How many enemies can this ammo damage before getting destroyed? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AmmoPierceAmount = 0;
};
