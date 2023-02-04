#pragma once

#include "CoreMinimal.h"

#include "ItemActorInterface.h"
#include "ViolentEnds/Interactable/InteractableInterface.h"

#include "ItemActor.generated.h"

class UItemBase;
class UBoxComponent;
class UStaticMeshComponent;

/**
 * Base class for item actor in world
 */
UCLASS()
class VIOLENTENDS_API AItemActor : public AActor, public IItemActorInterface, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AItemActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Core")
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Core")
	UItemBase* ItemHeld;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Collision")
	UBoxComponent* CollisionDetection;

protected:

	virtual void BeginPlay() override;

	// Inherited via IItemActorInterface
	virtual UItemBase* GetItem() const override;

	// Inherited via IInteractableInterface
	virtual void InteractWithObject(AActor* Source = nullptr) override;
};
