#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "UtilityPickup.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class APlayerCharacter;

UENUM(BlueprintType)
enum class EUtilityType : uint8
{
	None UMETA(DisplayName = "None"),
	Health UMETA(DisplayName = "Health"),
	Stamina UMETA(DisplayName = "Stamina"),
	StandardAmmo UMETA(DisplayName = "Standard Ammo"),
	BurstAmmo UMETA(DisplayName = "Burst Ammo"),
	ShotgunAmmo UMETA(DisplayName = "Shotgun Ammo"),
	SniperAmmo UMETA(DisplayName = "Sniper Ammo"),
};

UCLASS()
class VIOLENTENDS_API AUtilityPickup : public AActor
{
	GENERATED_BODY()

public:
	AUtilityPickup();

	friend class ABaseEnemy;

protected:
	virtual void BeginPlay() override;

	/* Sets the mesh based on the pickup type */
	void InitializeUtilityMesh();

	/* Randomizes the pickup type based on a weighted
	   loot table system using the TicketMap. */
	EUtilityType RandomizePickupType();

	/* Sets up the drop effect like the items have */
	UFUNCTION(BlueprintImplementableEvent)
	void PickupDropEffect();

	/* Increases an ammo stack size depending on the pickup type */
	void ReplenishAmmo();

	/* Binding function for overlaps, only executes fully if overlapping the Player */
	UFUNCTION()
	void OnUtilityOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* UtilityActorMesh;

	/* All possible meshes the pickup could be depending
	   on what type the pickup is. Randomized on spawn */
	UPROPERTY(EditAnywhere, Category = "Utility")
	TArray<UStaticMesh*> UtilityPossibleMeshes;

	/* Temporary materials for the basic sphere mesh
	   since we don't have all the assets right now */
	UPROPERTY(EditAnywhere, Category = "Utility")
	TArray<UMaterialInterface*> TemporaryTestingMaterials;

public:
	/* Determines what will get replenished upon overlap,
	   randomized in the enemy class upon spawn */
	UPROPERTY(BlueprintReadWrite, Category = "Utility")
	EUtilityType Type;

	/* Tickets determine the likelihood of a certain type being drawn upon randomization */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Ticket Amount")
	TMap<EUtilityType, int32> TicketMap;

private:
	/* All increases are percentages based off the corresponding stat for the Player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages",
		meta = (AllowPrivateAccess = true))
	float HealthReplenish = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages",
		meta = (AllowPrivateAccess = true))
	float StaminaReplenish = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages",
		meta = (AllowPrivateAccess = true))
	float StandardAmmoAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages",
		meta = (AllowPrivateAccess = true))
	float BurstAmmoAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages",
		meta = (AllowPrivateAccess = true))
	float ShotgunAmmoAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages",
		meta = (AllowPrivateAccess = true))
	float SniperAmmoAmount = 10;

	APlayerCharacter* PlayerReference;
};
