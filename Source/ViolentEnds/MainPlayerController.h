#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainPlayerController.generated.h"


class USpringArmComponent;
class UCameraComponent;


USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOccluded;
};


UCLASS()
class VIOLENTENDS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Materials")
	UMaterialInterface* FadeMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	class USpringArmComponent* ActiveSpringArm;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
	class UCameraComponent* ActiveCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion")
	bool IsOcclusionEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
	bool DebugLineTraces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Sweep Settings")
	float CapsuleRadius = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Sweep Settings")
	float CapsuleHeight = 10.f;
  
private:

	TMap<const AActor*, FCameraOccludedActor> OccludedActors;
	
	bool HideOccludedActor(const AActor* Actor);

	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;

	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);

	bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;

	void ForceShowOccludedActors();

	FORCEINLINE bool ShouldCheckCameraOcclusion() const
	{
		return IsOcclusionEnabled && FadeMaterial && ActiveCamera;
	}
  
public:

	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();

};
