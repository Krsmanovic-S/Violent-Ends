#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObject.h"
#include "LootSystem.h"
#include "Chest.generated.h"


UCLASS()
class VIOLENTENDS_API AChest : public AActor, public IInteractiveObject, public ILootSystem
{
	GENERATED_BODY()
	
public:	

	AChest();

	virtual void InteractPure() override;

protected:

	virtual void BeginPlay() override;

private:

	void InitializeChest();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ChestCollision;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* ChestMesh;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InteractionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Loot", meta = (AllowPrivateAccess = true))
	class UDataTable* ChestLootTable;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Loot", meta = (AllowPrivateAccess = true))
	int32 MaximalAmountOfItems = 1;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class UBaseItem*> InsideLoot; 
};
