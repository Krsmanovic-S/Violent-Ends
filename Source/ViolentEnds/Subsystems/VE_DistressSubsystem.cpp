#include "VE_DistressSubsystem.h"

#include "Camera/CameraComponent.h"
#include "FastNoiseWrapper.h"
#include "ViolentEnds/LogMacros.h"

UVE_DistressSubsystem::UVE_DistressSubsystem()
{
	RNG = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("FastNoiseGenerator"));
	RNG->SetupFastNoise(EFastNoise_NoiseType::Simplex,
		666,
		3,
		EFastNoise_Interp::Quintic,
		EFastNoise_FractalType::FBM,
		6,
		5,
		0.5,
		0.45,
		EFastNoise_CellularDistanceFunction::Euclidean,
		EFastNoise_CellularReturnType::CellValue);

	ScreenShakeConfig.Intensity = 30;
	ChromaticAberrationConfig.Intensity = 10;
}

void UVE_DistressSubsystem::AddDistress(double Amount)
{
	RawDistress += Amount;
}

void UVE_DistressSubsystem::UpdateFeedbacks()
{
	UpdateChromaticAberration();
	UpdateScreenShake();
}

void UVE_DistressSubsystem::UpdateScreenShake()
{
	if (!ScreenShakeConfig.IsEnabled()) { return; }

	auto NewOffsets = FVector(GetNoiseSample(0), GetNoiseSample(1), GetNoiseSample(2));
	NewOffsets *= ScreenShakeConfig.Intensity * EffectiveDistress;

	if (auto PlayerCamera = GetPlayerCamera(); PlayerCamera->IsValidLowLevel())
	{
		PlayerCamera->AddRelativeLocation(NewOffsets - LastAppliedCameraLocationOffset);
	}
	else { LOG_WARNING(LogTemp, "Could not get camera"); }

	LastAppliedCameraLocationOffset = NewOffsets;
}

void UVE_DistressSubsystem::UpdateChromaticAberration()
{
	if (!ChromaticAberrationConfig.IsEnabled()) { return; }
	if (ExcessDistress <= 0) { return; }

	double NewValue = ChromaticAberrationConfig.Intensity * ExcessDistress;

	if (auto PlayerCamera = GetPlayerCamera(); PlayerCamera->IsValidLowLevel())
	{
		PlayerCamera->PostProcessSettings.SceneFringeIntensity = NewValue;
	}
	else { LOG_WARNING(LogTemp, "Could not get camera"); }
}

UCameraComponent* UVE_DistressSubsystem::GetPlayerCamera()
{
	if (CachedPlayerCamera.IsValid()) { return CachedPlayerCamera.Get(); }

	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController)) { return nullptr; }
	if (!IsValid(PlayerController->AcknowledgedPawn)) { return nullptr; }

	auto CameraComponent = PlayerController->AcknowledgedPawn->GetComponentByClass(UCameraComponent::StaticClass());
	if (!IsValid(CameraComponent)) { return nullptr; }

	CachedPlayerCamera = Cast<UCameraComponent>(CameraComponent);
	return CachedPlayerCamera.Get();
}

void UVE_DistressSubsystem::Tick(float DeltaTime)
{
	Time += DeltaTime;

	if (EffectiveDistress <= 0 && RawDistress <= 0) { return; }

	RawDistress -= DeltaTime * DecayMultiplier * FMath::Clamp(RawDistress, 0.1, 1);
	RawDistress = FMath::Max(RawDistress, 0);

	double ClampedDistress = FMath::Clamp(RawDistress, 0, 1);
	double NewEffectiveDistress = FMath::Pow(ClampedDistress, 2);
	double NewExcessDistress = RawDistress < 1 ? 0 : FMath::LogX(10, RawDistress);

	EffectiveDistress = FMath::FInterpTo(EffectiveDistress,
		NewEffectiveDistress,
		DeltaTime,
		NewEffectiveDistress > EffectiveDistress ? InterpolationSpeeds[0] : InterpolationSpeeds[1]);
	ExcessDistress = FMath::FInterpTo(ExcessDistress,
		NewExcessDistress,
		DeltaTime,
		NewExcessDistress > ExcessDistress ? InterpolationSpeeds[0] : InterpolationSpeeds[1]);

	UpdateFeedbacks();
}

TStatId UVE_DistressSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTRSTimeScaleManager, STATGROUP_Tickables);
}

float UVE_DistressSubsystem::GetNoiseSample(float OnTrack) const
{
	return RNG->GetNoise2D(Time, OnTrack * 1e3);
}