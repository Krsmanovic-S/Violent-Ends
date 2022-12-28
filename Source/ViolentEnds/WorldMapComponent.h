#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "WorldMapComponent.generated.h"

class UTexture2D;
class ATargetPoint;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIOLENTENDS_API UWorldMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWorldMapComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	float GetMapHeight() const;

	UFUNCTION(BlueprintCallable)
	float GetMapWidth() const;

	UFUNCTION(BlueprintCallable)
	FVector2D ConvertLocationToMapLocation(FVector InputLocation);

public:
	UPROPERTY(BlueprintReadWrite)
	float MapWidth;

	UPROPERTY(BlueprintReadWrite)
	float MapHeight;

	UPROPERTY(BlueprintReadWrite)
	AActor* UpperLeftCorner;

	UPROPERTY(BlueprintReadWrite)
	AActor* LowerRightCorner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map Properties")
	TSubclassOf<ATargetPoint> TargetPointClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Properties")
	UTexture2D* MapTexture;
};
