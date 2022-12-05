#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemEnumsAndStructs.h"
#include "LootSystem.generated.h"


class UBaseItem;
class UDataTable;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULootSystem : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API ILootSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UBaseItem* PickRandomItem(UDataTable* LootTable, int32 CurrentPlayerLevel);
	virtual EItemTier RandomizeTier(int32 CurrentPlayerLevel);
	virtual EItemRarity RandomizeRarity();
	virtual void InitializeLoot(TArray<UBaseItem*>& Loot, UDataTable* LootTable, int32 MaximalItems, int32 CurrentPlayerLevel);
};
