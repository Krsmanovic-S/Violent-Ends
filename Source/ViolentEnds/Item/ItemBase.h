#pragma once 

#include "CoreMinimal.h"

#include "ItemTypes.h"
#include "GameplayTagContainer.h"

#include "ItemBase.generated.h"

UCLASS(Blueprintable)
class VIOLENTENDS_API UItemBase: public UObject {

	GENERATED_BODY()

public:

	UItemBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	int32 ItemValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	FText ItemDescriptions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	int32 ItemMaxStack;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item")
	FGameplayTag ItemTag;

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool TryUseItem(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool TryUseItemSecondary(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanBeUsed(AActor* Target);

	virtual bool TryUseItem_Implementation(AActor* Target);

	virtual bool TryUseItemSecondary_Implementation(AActor* Target);

	virtual bool CanBeUsed_Implementation(AActor* Target);
};