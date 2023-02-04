#pragma once

#include "CoreMinimal.h"
#include "ViolentEnds/Interactable/InteractableInterface.h"
#include "InteractableChest.generated.h"

class UBoxComponent;
class UItemBase;

UCLASS()
class VIOLENTENDS_API AInteractableChest: public AActor, public IInteractableInterface {

	GENERATED_BODY()

public:

	AInteractableChest();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	UStaticMeshComponent* ChestMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	UBoxComponent* InteractableArea;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	USceneComponent* ItemSpawnLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	TArray<UItemBase*> ContainedItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable|Door")
	bool bHasBeenOpened;

	UFUNCTION(BlueprintNativeEvent)
	void OpenChest();

	void OpenChest_Implementation();

	// Inherited via IInteractableInterface
	virtual void InteractWithObject(AActor* Source) override;

};