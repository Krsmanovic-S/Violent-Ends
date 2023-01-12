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
		auto ActorInfo = CharacterASC->AbilityActorInfo.Get();

		if (ActorInfo)
		{
			ActorInfo->AvatarActor = this;
			ActorInfo->OwnerActor = this;
			ActorInfo->AnimInstance = MakeWeakObjectPtr(GetMesh()->GetAnimInstance());
			ActorInfo->MovementComponent = MakeWeakObjectPtr(GetCharacterMovement());
		}

		UGE_CharacterInitialization* CharacterStatEffect = NewObject<UGE_CharacterInitialization>(this, CharacterDefaultStats);

#if WITH_EDITORONLY_DATA
		CharacterStatEffect = UGE_CharacterInitialization::CreateEffectFromHelperStruct(EditorCharacterStat);
#endif // WITH_EDITORONLY_DATA

		FGameplayEffectSpec EffectSpec;
		EffectSpec.Def = CharacterStatEffect;
		EffectSpec.Duration;

		CharacterASC->ApplyGameplayEffectSpecToSelf(EffectSpec);
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

void ACharacterBase::OnCharacterMovementSpeedChanged_Implementation(const FOnAttributeChangeData& Data) {
	OnCharacterMovementSpeedChanged(Data.OldValue, Data.NewValue);

	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;

}
