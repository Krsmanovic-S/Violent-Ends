#pragma once

UENUM(BlueprintType)
enum class EWeaponFireMode: uint8 {

	Single UMETA(DisplayName = "Single"),
	Burst UMETA(DisplayName = "Burst"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Auto UMETA(DisplayName = "Auto"),
	Charge UMETA(DisplayName = "Charge")
};