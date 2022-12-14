#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Shield.generated.h"

class UStaticMeshComponent;
class UEntityStats;

UCLASS(BlueprintType, Blueprintable)
class VIOLENTENDS_API AShield : public AActor
{
	GENERATED_BODY()

	friend class AProjectile;

public:
	AShield();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UEntityStats* ShieldStats;

	/* Which actor summoned the shield? This determines who can shoot through it */
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), meta = (AllowPrivateAccess = true))
	AActor* ShieldOwner;

	/* Should bullets that came from the player pass through? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIgnorePlayerShield = false;

	/* Should bullets that came from enemies pass through? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIgnoreEnemyShield = false;
};
