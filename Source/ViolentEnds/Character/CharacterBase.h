#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "ViolentEnds/GameplaySystem/GameplayEffect/CharacterInitializeEffect/GE_CharacterInitialization.h"
#include "ViolentEnds/GameplaySystem/Combat/CombatInterface.h"

#include "CharacterBase.generated.h"

class UVE_ASC;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDeath, ACharacter*, DeadCharacter);

UCLASS()
class VIOLENTENDS_API ACharacterBase :
	public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/**
	* Delegate for when character is killed
	*/
	UPROPERTY(BlueprintAssignable, Category = "Character")
	FCharacterDeath OnCharacterDeath;

	/**
	* Character's (custom) ability system component
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	UVE_ASC* CharacterASC;

	/**
	* Character's default attribute
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	TSubclassOf<UGE_CharacterInitialization> CharacterDefaultStats;

	/**
	* Character's array of attribute sets to assign to this ASC
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	TArray<TSubclassOf<UAttributeSet>> RegisteredAttributeSets;

	/**
	* Attempt to apply the effect of Effect class to this character
	*/
	UFUNCTION(BlueprintCallable)
	virtual bool TryApplyEffectToSelf(UClass* Effect);


protected:

	virtual void BeginPlay() override;

	// Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**
	* Blueprint implementable event for event health changed
	* @param OldValue The old health value
	* @param NewValue The new health value
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterHealthChanged(float OldValue, float NewValue);	
	
	/**
	* Blueprint implementable event for event movement speed changed
	* @param OldValue The old movement speed value
	* @param NewValue The new movement speed value
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterMovementSpeedChanged(float OldValue, float NewValue);

	/**
	* Blueprint implementable event for when character is hit
	* @param HitTag The gameplay tag of the hit
	*/
	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterHit(FGameplayTag HitTag);

	void NativeCharacterHealthChanged(const FOnAttributeChangeData& Data);
	void NativeOnCharacterMovementSpeedChanged(const FOnAttributeChangeData& Data);
	void OnCharacterHit_Implementation(FGameplayTag HitTag);



// Inherited via ICombatInterface
	virtual bool TryUseAbilityWithTag(const FGameplayTagContainer& AbilityTag) override;
	virtual bool TryUseDefaultAttack() override;
};