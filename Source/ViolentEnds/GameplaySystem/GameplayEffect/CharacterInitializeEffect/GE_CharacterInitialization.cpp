#include "GE_CharacterInitialization.h"

UGE_CharacterInitialization::UGE_CharacterInitialization() {
	DurationPolicy = EGameplayEffectDurationType::Infinite;
}

UGE_CharacterInitialization* UGE_CharacterInitialization::CreateEffectFromHelperStruct(
	FCharacterInitialStatStruct& InitialStats)
{

	UGE_CharacterInitialization* RetVal = NewObject<UGE_CharacterInitialization>();

	TArray<FCharacterStatStruct>& CharacterStats = InitialStats.CharacterStats;
	for (auto& Item : CharacterStats) {
		FGameplayModifierInfo ModInfo;

		ModInfo.Attribute = Item.Attribute;
		ModInfo.Magnitude = Item.Value;
		ModInfo.ModifierOp = EGameplayModOp::Override;

		RetVal->Modifiers.Add(ModInfo);
	}

	return RetVal;
}
