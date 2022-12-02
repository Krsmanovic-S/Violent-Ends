#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItemActor.generated.h"

class UBaseItem;
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
	
	void RandomizeIndividualStat(float& CurrentStat, FVector2D CurrentRange);
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
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent* ItemWidgetName;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bWasItemInitialized;
};
