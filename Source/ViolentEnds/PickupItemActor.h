#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItemActor.generated.h"

class UBaseItem;
class USkeletalMeshComponent;
class UBoxComponent;
class UWidgetComponent;

UCLASS()
class VIOLENTENDS_API APickupItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupItemActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InitializePickupProperties();
	
	// Calculates the bottom and upper bound (% wise) that we will take into
	// consideration for the bounds of a stat that we are randomizing.
	FVector2D StatsRangeOnTier();

	// Calculates a new percentile based range which will determine the new upper and lower
	// bound of a stat. This comes after the tier function has processed a range first.
	FVector2D StatsRangeOnRarity();

	void RandomizeIndividualStat(float& CurrentStat, FVector2D CurrentRange, const FVector2D& TierRange, const FVector2D& RarityRange);
	void RandomizeItemStats();
	
public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	UBaseItem* ContainedItem;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	int32 AmountToAdd;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent* ItemWidgetName;

	UPROPERTY(BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	bool bWasItemInitialized;
};
