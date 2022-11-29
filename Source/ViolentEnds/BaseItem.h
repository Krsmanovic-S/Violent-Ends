#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemEnumsAndStructs.h"
#include "BaseItem.generated.h"


UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class VIOLENTENDS_API UBaseItem : public UObject
{
	GENERATED_BODY()

public:

	UBaseItem();

	virtual class UWorld* GetWorld() const { return this->World; };

	UFUNCTION(BlueprintCallable)
	void InitializeTooltipMap();

	UFUNCTION(BlueprintCallable)
	virtual void Use(class APlayerCharacter* Player);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class APlayerCharacter* Player);

	UPROPERTY(Transient)
	class UWorld* World;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	class UStaticMesh* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	class UTexture2D* ItemThumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	bool bIsUsable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	bool bIsEquipment = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	bool bIsQuestItem = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	int32 ItemMaxStack;

	UPROPERTY(BlueprintReadOnly)
	bool bWasInitialized = false;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemCurrentStack;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemSlotIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Stats")
	FItemStatsRange ItemStatsRange;

	UPROPERTY(BlueprintReadWrite)
	FItemStats ItemStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemType Type;

	UPROPERTY(BlueprintReadWrite)
	EItemTier Tier;

	UPROPERTY(BlueprintReadWrite)
	EItemRarity Rarity = EItemRarity::Common;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, float> MapForTooltips;

	UPROPERTY(BlueprintReadWrite)
	class UInventoryComponent* OwningInventory;

};