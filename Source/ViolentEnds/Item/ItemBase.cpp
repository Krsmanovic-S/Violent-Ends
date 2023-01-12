#include "ItemBase.h"

UItemBase::UItemBase() {

	ItemTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Item"));
}

bool UItemBase::TryUseItem(AActor* Target)
{
	return TryUseItem_Implementation(Target);
}

bool UItemBase::TryUseItemSecondary(AActor* Target)
{
	return TryUseItemSecondary_Implementation(Target);
}

bool UItemBase::CanBeUsed(AActor* Target)
{
	return CanBeUsed_Implementation(Target);
}

bool UItemBase::TryUseItem_Implementation(AActor* Target)
{
	bool Result = CanBeUsed_Implementation(Target);

	return Result;
}

bool UItemBase::TryUseItemSecondary_Implementation(AActor* Target)
{
	return false;
}

bool UItemBase::CanBeUsed_Implementation(AActor* Target)
{
	return false;
}
