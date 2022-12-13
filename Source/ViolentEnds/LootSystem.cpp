#include "LootSystem.h"

#include "BaseItem.h"
#include "Engine/DataTable.h"

// Add default functionality here for any ILootSystem functions that are not pure virtual.
UBaseItem* ILootSystem::PickRandomItem(UDataTable* LootTable, int32 CurrentPlayerLevel)
{
	if (LootTable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No loot table set for ILootSystem::PickRandomItem()."));
		return nullptr;
	}

	TArray<FName> AllTableRows = LootTable->GetRowNames();

	int32 RandomRow = FMath::RandRange(0, AllTableRows.Num() - 1);
	float RandomNumber = FMath::RandRange(0, 100);

	// Select a Random Row in the Data Table
	FItemDataTable* SelectedRow = LootTable->FindRow<FItemDataTable>(AllTableRows[RandomRow], "");

	if (SelectedRow && SelectedRow->MinimalPlayerLevel <= CurrentPlayerLevel
		&& SelectedRow->ChanceToSpawn >= RandomNumber && CurrentPlayerLevel <= SelectedRow->MaximalPlayerLevel)
	{
		return SelectedRow->ItemClass->GetDefaultObject<UBaseItem>();
	}
	return nullptr;
}

EItemTier ILootSystem::RandomizeTier(int32 CurrentPlayerLevel)
{
	int32 CurrentTier = FMath::Max(CurrentPlayerLevel / 10, 1);
	int32 MinimalTier = FMath::Max(CurrentTier - 1, 1);
	int32 MaximalTier = FMath::Min(CurrentTier + 1, 5);

	// 20% chance for 1 tier lower
	// 70% chance for same tier
	// 10% chance for 1 tier higher
	TArray<int32> TierArray = { MinimalTier, MinimalTier, CurrentTier, CurrentTier, CurrentTier, CurrentTier,
		CurrentTier, CurrentTier, CurrentTier, MaximalTier };

	int32 SelectedTier = TierArray[FMath::RandRange(0, TierArray.Num() - 1)];

	switch (SelectedTier)
	{
		case 1:
			return EItemTier::Tier1;
		case 2:
			return EItemTier::Tier2;
		case 3:
			return EItemTier::Tier3;
		case 4:
			return EItemTier::Tier4;
	}

	return EItemTier::Tier5;
}

EItemRarity ILootSystem::RandomizeRarity()
{
	int32 Random = FMath::RandRange(0, 100);

	// 3% for Legendary
	// 12% for Epic
	// 18% for Rare
	// 27% for Uncommon
	// 40% for Common

	if (Random < 40) { return EItemRarity::Common; }
	else if (Random < 67) { return EItemRarity::Uncommon; }
	else if (Random < 85) { return EItemRarity::Rare; }
	else if (Random < 98) { return EItemRarity::Epic; }

	return EItemRarity::Legendary;
}

void ILootSystem::InitializeLoot(
	TArray<UBaseItem*>& Loot, UDataTable* LootTable, int32 MaximalItems, int32 CurrentPlayerLevel)
{
	if (LootTable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No loot table selected for InitializeLoot() method."));
		return;
	}

	int32 AmountToGenerate = FMath::RandRange(1, MaximalItems);

	int32 AddedItems = 0;
	int32 BreakPoint = 50;

	UBaseItem* InputItem;
	UBaseItem* CopyItem;

	// Either run until we generated the maximal amout or
	// end after a fixed number of iterations.
	while (AddedItems != AmountToGenerate && BreakPoint != 0)
	{
		// Select randomly from a data table
		InputItem = this->PickRandomItem(LootTable, CurrentPlayerLevel);

		if (InputItem != NULL)
		{
			CopyItem = DuplicateObject(InputItem, NULL);

			// Randomize Tier
			CopyItem->Tier = RandomizeTier(CurrentPlayerLevel);

			// Randomize Rarity
			CopyItem->Rarity = RandomizeRarity();

			Loot.Add(CopyItem);
			AddedItems++;
			BreakPoint--;
		}
		else { BreakPoint--; }
	}
}