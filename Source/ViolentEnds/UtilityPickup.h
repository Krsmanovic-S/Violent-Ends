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
	None			UMETA(DisplayName = "None"),
    Health          UMETA(DisplayName = "Health"),  
    Stamina     	UMETA(DisplayName = "Stamina"),
    StandardAmmo    UMETA(DisplayName = "Standard Ammo"),
    BurstAmmo       UMETA(DisplayName = "Burst Ammo"),
    ShotgunAmmo     UMETA(DisplayName = "Shotgun Ammo"),
	SniperAmmo      UMETA(DisplayName = "Sniper Ammo"),
};

UCLASS()
class VIOLENTENDS_API AUtilityPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AUtilityPickup();

	// Sets the mesh based on the pickup type.
	void InitializeUtilityMesh();

	// Randomizes the pickup type based on a weighted
	// loot table system using the TicketMap.
	EUtilityType RandomizePickupType();

	UFUNCTION(BlueprintImplementableEvent)
	void PickupDropEffect();

protected:
	
	virtual void BeginPlay() override;

	void ReplenishAmmo();

	UFUNCTION()
	void OnUtilityOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
	TArray<UStaticMesh*> UtilityPossibleMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
	TArray<UMaterialInterface*> TemporaryTestingMaterials;

	UPROPERTY(BlueprintReadWrite, Category = "Utility")
	EUtilityType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Ticket Amount")
	TMap<EUtilityType, int32> TicketMap;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* UtilityActorMesh;

	APlayerCharacter* PlayerReference;

	// ------------------Replenish Amounts (%)---------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages", meta = (AllowPrivateAccess = true))
	float HealthReplenish = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages", meta = (AllowPrivateAccess = true))
	float StaminaReplenish = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages", meta = (AllowPrivateAccess = true))
	float StandardAmmoAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages", meta = (AllowPrivateAccess = true))
	float BurstAmmoAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages", meta = (AllowPrivateAccess = true))
	float ShotgunAmmoAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Replenish Percentages", meta = (AllowPrivateAccess = true))
	float SniperAmmoAmount = 10;
	// -----------------------------------------------------------
};
