#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "VE_DistressSubsystem.generated.h"

class UCameraComponent;
class UFastNoiseWrapper;

USTRUCT()
struct VIOLENTENDS_API FFeedbackConfig
{
	GENERATED_BODY()

	UPROPERTY()
	double Intensity = 1.0;

	bool IsEnabled() const { return bEnabled && Intensity > 0; }

private:
	UPROPERTY()
	bool bEnabled = true;
};

/**
 * 'Distress' subsystem that handles feedback (screen shake, force feedback, etc)
 * - Add distress to intensify feedback
 * - Feedback level slowly decays
 * - Drive the various configurable feedbacks continuously
 */
UCLASS()
class VIOLENTENDS_API UVE_DistressSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	UVE_DistressSubsystem();

public:
	UFUNCTION(BlueprintCallable)
	/** Add distress amount (probably between 0 to 1), which will decay and drive feedback effects */
	void AddDistress(double Amount);

	/** Update the applied feedbacks with latest values */
	void UpdateFeedbacks();

	/** RawDistress decreases by this much per second */
	double DecayMultiplier = 1.5;

	/** Interpolation speed when going {upwards, downwards}.
	 * Upwards being higher seems to work better, otherwise adding lots at once causes a jarring effect
	 */
	TArray<double> InterpolationSpeeds = { 5, 20 };

protected: // State/ internals
	void UpdateScreenShake();

	void UpdateChromaticAberration();

	UCameraComponent* GetPlayerCamera();

	UPROPERTY()
	TSoftObjectPtr<UCameraComponent> CachedPlayerCamera;

	UPROPERTY()
	/** Internal tracker of the current value, used to derive effective and excess distresses */
	double RawDistress;

	UPROPERTY()
	/** Essentially RawDistress ^ 2, when RawDistress is between 0 and 1 */
	double EffectiveDistress;

	UPROPERTY()
	/** Excess distress to drive rarer effects by */
	double ExcessDistress;

	UPROPERTY()
	/** Track time for altering noise sampling by */
	double Time = 0;

	UPROPERTY()
	FFeedbackConfig ScreenShakeConfig;

	UPROPERTY()
	FVector LastAppliedCameraLocationOffset;

	UPROPERTY()
	FFeedbackConfig ChromaticAberrationConfig;

public: // Tick
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

protected: // Noise generation
	UFUNCTION()
	float GetNoiseSample(float OnTrack) const;

	UPROPERTY()
	UFastNoiseWrapper* RNG;
};