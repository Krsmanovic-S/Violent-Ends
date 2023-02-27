#pragma once

#include "CoreMinimal.h"

#include "EnvironmentalHazardBase.h"

#include "TrapBase.generated.h"

class UGameplayEffect;
class UShapeComponent;
class UNiagaraSystem;
class UAudioComponent;
enum class EHazardShape: uint8;

/**
 * The base class for a trap that usually is activated when a valid actor enters its activation range
 * As trap can come with different shapes, user should add primitive collision component as fit
 */
UCLASS(Abstract)
class VIOLENTENDS_API ATrapBase : public AEnvironmentalHazardBase
{
	GENERATED_BODY()

public:

	ATrapBase();

#if WITH_EDITORONLY_DATA
	/**
	 * The display mesh of the prop
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	EHazardShape HazardShape;

#endif // WITH_EDITORONLY_DATA



	/**
	 * The display mesh of the prop
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UStaticMeshComponent* TrapMeshComponent;

	/**
	 * The collision of the prop
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UShapeComponent* ActivationDetection;

	/**
	* The SFX/VFX effect spawn location
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	USceneComponent* VisualEffectSpawnLocation;

	/**
	 * The effect to play when this is activated
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UNiagaraSystem* ActivationVFX;

	/**
	 * The sound effect when this is activated
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UAudioComponent* ActivationAudioComponent;

	/**
	 * The gameplay effect used to apply to the targets in range
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	TSubclassOf<UGameplayEffect> TrapEffect;

	/**
	 * Optional: the damage falloff based on the distance of target(s) from this object
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hazard")
	UCurveFloat* DamageFalloff;

	UFUNCTION(BlueprintNativeEvent, Category = "Hazard")
	void OnActivationDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

protected:
	virtual void OnActivationDetected_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Activate() override;
	virtual bool CanBeReused() override;
	virtual void OnActivated_Implementation() override;
};