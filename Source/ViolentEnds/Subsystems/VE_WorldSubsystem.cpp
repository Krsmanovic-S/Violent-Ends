#include "VE_WorldSubsystem.h"

#include "AbilitySystemInterface.h"
#include "ViolentEnds/GameplaySystem/Component/VE_ASC.h"

UVE_WorldSubsystem::UVE_WorldSubsystem()
{
	WorldAbilitySystemComponent = CreateDefaultSubobject<UVE_ASC>(TEXT("WorldAbilitySystemComponent"));
}

bool UVE_WorldSubsystem::TryApplyEffectToTargetActor(TSubclassOf<UGameplayEffect> EffectToApply, AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC;
	bool Success = TryGetAbilitySystemComponentFromActor(TargetActor, TargetASC);

	if (!Success) { return false; }

	FGameplayEffectContextHandle ContextHandle = WorldAbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle =
		WorldAbilitySystemComponent->MakeOutgoingSpec(EffectToApply, 0, ContextHandle);

	FActiveGameplayEffectHandle EffectHandle;

	if (EffectSpecHandle.IsValid())
	{
		EffectHandle =
			WorldAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
	}

	return EffectHandle.IsValid();
}

bool UVE_WorldSubsystem::TryApplyEffectToTargetActorWithContext(
	TSubclassOf<UGameplayEffect> EffectToApply, AActor* TargetActor, FGameplayEffectContextHandle& ContextHandle)
{
	UAbilitySystemComponent* TargetASC;
	bool Success = TryGetAbilitySystemComponentFromActor(TargetActor, TargetASC);

	if (!Success) { return false; }

	FGameplayEffectSpecHandle EffectSpecHandle =
		WorldAbilitySystemComponent->MakeOutgoingSpec(EffectToApply, 0, ContextHandle);

	FActiveGameplayEffectHandle EffectHandle;

	if (EffectSpecHandle.IsValid())
	{
		EffectHandle =
			WorldAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
	}

	return EffectHandle.IsValid();
}

bool UVE_WorldSubsystem::TryApplyDynamicEffectToTargetActor(UGameplayEffect* EffectToApply, AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC;
	bool Success = TryGetAbilitySystemComponentFromActor(TargetActor, TargetASC);

	if (!Success) { return false; }

	FGameplayEffectContextHandle ContextHandle = WorldAbilitySystemComponent->MakeEffectContext();

	FActiveGameplayEffectHandle EffectHandle;

	EffectHandle = WorldAbilitySystemComponent->ApplyGameplayEffectToTarget(EffectToApply, TargetASC, 0, ContextHandle);

	return EffectHandle.IsValid();
}

bool UVE_WorldSubsystem::TryApplyDynamicEffectToTargetActorWithContext(
	UGameplayEffect* EffectToApply, AActor* TargetActor, FGameplayEffectContextHandle& ContextHandle)
{
	UAbilitySystemComponent* TargetASC;
	bool Success = TryGetAbilitySystemComponentFromActor(TargetActor, TargetASC);

	if (!Success) { return false; }

	FActiveGameplayEffectHandle EffectHandle;

	EffectHandle = WorldAbilitySystemComponent->ApplyGameplayEffectToTarget(EffectToApply, TargetASC, 0, ContextHandle);

	return EffectHandle.IsValid();
}

UAbilitySystemComponent* UVE_WorldSubsystem::GetWorldASC()
{
	return WorldAbilitySystemComponent;
}

bool UVE_WorldSubsystem::TryGetAbilitySystemComponentFromActor(
	AActor* TargetActor, UAbilitySystemComponent*& OutTargetAbilitySystemComponent)
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetActor);
	if (!AbilitySystemInterface) { return false; }

	OutTargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	if (!OutTargetAbilitySystemComponent) { return false; }

	return true;
}

void UVE_WorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (!WorldAbilitySystemComponent)
	{
		WorldAbilitySystemComponent = NewObject<UVE_ASC>(GetWorld(), TEXT("WorldAbilitySystemComponent"));
	}
}
