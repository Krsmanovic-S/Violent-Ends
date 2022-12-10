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

	/* Initializes the skeletal mesh based on the contained item
	   and randomizes the stats if the item hasn't been initialized */
	UFUNCTION(BlueprintCallable)
	void InitializePickupProperties();

	/* Calculates the bottom and upper bound (% wise) that we will take into
	   consideration for the bounds of a stat that we are randomizing */
	FVector2D StatsRangeOnTier();

	/* Calculates a new percentile based range which will determine the new upper and lower
	   bound of a stat. This comes after the tier function has processed a range first */
	FVector2D StatsRangeOnRarity();

	/* Takes a single stat and passes its range through the tier and rarity randomization
	   after which it finally picks the resulting value for the passed stat */
	void RandomizeIndividualStat(
		float& CurrentStat, FVector2D CurrentRange, const FVector2D& TierRange, const FVector2D& RarityRange);

	/* Goes through all the possible stats an item can have and randomizes them individually */
	void RandomizeItemStats();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* SkeletalMeshComp;

	/* Display widget for showing the name of the contained item */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent* ItemWidgetName;

	/* Item that this pickup actor represents, can be set as an instance or via blueprints on spawn */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	UBaseItem* ContainedItem;

	/* Amount of the item to add to upon overlap, this is here to not mess with the item stack or the
	   for-loop in blueprints which is used to add a certain amount of this item to the inventory */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	int32 AmountToAdd = 1;

	/* Determines if we should randomize an item stats or were they
	   already generated. Prevents re-randomizing already existing items */
	UPROPERTY(BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	bool bWasItemInitialized;
};
