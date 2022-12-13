#pragma once

#include "CoreMinimal.h"

#include "ItemEnumsAndStructs.h"
#include "UObject/Interface.h"

#include "LootSystem.generated.h"

class UBaseItem;
class UDataTable;

UINTERFACE(MinimalAPI)
class ULootSystem : public UInterface
{
	GENERATED_BODY()
};

class VIOLENTENDS_API ILootSystem
{
	GENERATED_BODY()

public:
	/**
	 * Select a random row from the input data table and roll a random
	   integer against that rows ChanceToSpawn section

	 * @param LootTable table from which we pick the item
	 * @param CurrentPlayerLevel checked against the min and max level requirements in the data table
	 * @return item from the data table if roll was high enough, otherwise nullptr
	 */
	virtual UBaseItem* PickRandomItem(UDataTable* LootTable, int32 CurrentPlayerLevel);

	/**
	 * Generate the tier for an item based on the player level,
	   uses a weighted loot table system for the draw (tickets)

	 * @param CurrentPlayerLevel determines the current tier of this level
	 * @return Current levels tier +/- one tier
	 */
	virtual EItemTier RandomizeTier(int32 CurrentPlayerLevel);

	/**
	 * Roll a random integer and based on its value
	   between 0 and 100, return the selected rarity

	 * @return rarity between Common and Legendary
	 */
	virtual EItemRarity RandomizeRarity();

	/**
	 * Fill out the input Loot with a random set of items that are
	   chosen from a loot table. How many items are drawn
	   depends on a random integer between 0 and MaximalItems

	 * @param Loot array that will get filled up
	 * @param LootTable table which we look at to pick the items
	 * @param MaximalItems the highest possible items that we can generate
	 * @param CurrentPlayerLevel passed onto other methods for picking/calculating the tier
	 */
	virtual void InitializeLoot(
		TArray<UBaseItem*>& Loot, UDataTable* LootTable, int32 MaximalItems, int32 CurrentPlayerLevel);
};
