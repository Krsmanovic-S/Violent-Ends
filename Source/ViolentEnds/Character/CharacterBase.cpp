#include "CharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ViolentEnds/GameplaySystem/AttributeSet/AttributeSet_BaseAttribute.h"
#include "ViolentEnds/GameplaySystem/Component/VE_ASC.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterASC = CreateDefaultSubobject<UVE_ASC>(TEXT("CharacterAbilitySystemComponent"));
	CharacterASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_BaseAttribute::GetHealthAttribute())
		.AddUObject(this, &ACharacterBase::OnCharacterHealthChanged_Implementation);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialize actor info for ability system
	if (CharacterASC)
	{
		CharacterASC->InitAbilityActorInfo(this, this);

		auto ActorInfo = CharacterASC->AbilityActorInfo.Get();
		if (ActorInfo)
		{
			ActorInfo->AnimInstance = MakeWeakObjectPtr(GetMesh()->GetAnimInstance());
			ActorInfo->MovementComponent = MakeWeakObjectPtr(GetCharacterMovement());
		}

		// Register list of attribute set to the ASC
		for (auto& AttributeSetClass : RegisteredAttributeSets) {
			auto AttributeSet = NewObject<UAttributeSet>(this, AttributeSetClass);
			CharacterASC->AddAttributeSetSubobject(AttributeSet);
		}

		if (CharacterDefaultStats)
		{
			FGameplayEffectContextHandle ContextHandle = CharacterASC->MakeEffectContext();
			FGameplayEffectSpecHandle EffectSpec =
				CharacterASC->MakeOutgoingSpec(CharacterDefaultStats, 0, ContextHandle);

			if (EffectSpec.IsValid()) { auto Handle = CharacterASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get()); }
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("AbilitySystemComponent is null")));
		}
	}
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return CharacterASC;
}

void ACharacterBase::OnCharacterHealthChanged_Implementation(const FOnAttributeChangeData& Data)
{
	OnCharacterHealthChanged(Data.OldValue, Data.NewValue);

	if (Data.NewValue <= 0.1f || FMath::IsNearlyZero(Data.NewValue))
	{
		OnCharacterDeath.Broadcast(this);

		// Since the character is considered dead, unbind all function from delegate
		CharacterASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_BaseAttribute::GetHealthAttribute())
			.RemoveAll(this);
	}
}

void ACharacterBase::OnCharacterMovementSpeedChanged_Implementation(const FOnAttributeChangeData& Data)
{
	OnCharacterMovementSpeedChanged(Data.OldValue, Data.NewValue);

	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

bool ACharacterBase::TryUseAbilityWithTag(const FGameplayTagContainer& AbilityTag)
{
	if (!CharacterASC) { return false; }

	bool Result = CharacterASC->TryActivateAbilitiesByTag(AbilityTag);

	UE_LOG(LogTemp, Display, TEXT("%s attempt to use ability with tag %s: %s"), *GetFName().ToString(),
		*AbilityTag.ToString(), Result ? *FString(TEXT("Success")) : *FString(TEXT("Failed")))

	return Result;
}

bool ACharacterBase::TryUseDefaultAttack()
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Ability.Attack.Default")));

	return TryUseAbilityWithTag(TagContainer);
}
