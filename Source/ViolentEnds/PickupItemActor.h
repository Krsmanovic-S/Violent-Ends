#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItemActor.generated.h"


UCLASS()
class VIOLENTENDS_API APickupItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupItemActor();

protected:
	virtual void BeginPlay() override;

	void RandomizeIndividualStat(float& CurrentStat, FVector2D CurrentRange);
	void RandomizeItemStats();
	
public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	class UBaseItem* ContainedItem;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = true))
	int32 AmountToAdd;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UWidgetComponent* ItemWidgetName;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bWasItemInitialized;
};
