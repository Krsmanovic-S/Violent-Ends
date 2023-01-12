#pragma once

#include "CoreMinimal.h"

#include "GameplayEffect.h"

#include "GE_CharacterInitialization.generated.h"



/**
* Set of affected attribute and it's magnitude
*/
USTRUCT(BlueprintType)
struct VIOLENTENDS_API FCharacterStatStruct
{
	GENERATED_BODY()

public:

	/**
	* The attribute to set the character to
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Stats")
	FGameplayAttribute Attribute;

	/**
	* The value of the attribute
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Stats")
	float Value;

};

/**
* Helper struct to create character initial stats in Editor
*/
USTRUCT(BlueprintType)
struct VIOLENTENDS_API FCharacterInitialStatStruct
{
	GENERATED_BODY()

public:
	/*
	* The list of all attributes to apply to the character
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Stats")
	TArray<FCharacterStatStruct> CharacterStats;

	bool IsValid() {return (CharacterStats.Num() > 0);};
};

UCLASS()
class VIOLENTENDS_API UGE_CharacterInitialization : public UGameplayEffect
{
	GENERATED_BODY()

public:

	UGE_CharacterInitialization();

	static UGE_CharacterInitialization* CreateEffectFromHelperStruct(FCharacterInitialStatStruct& InitialStats);
};