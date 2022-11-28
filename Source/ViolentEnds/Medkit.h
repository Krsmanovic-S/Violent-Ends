#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Medkit.generated.h"


UCLASS()
class VIOLENTENDS_API UMedkit : public UBaseItem
{
	GENERATED_BODY()
	
public:

	UMedkit();

protected:
	virtual void Use(class APlayerCharacter* Player) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FlatHealAmount = 20;
};
