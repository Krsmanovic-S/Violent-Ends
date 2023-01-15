#include "QuestObjective.h"
#include "Components/BoxComponent.h"
#include "ViolentEnds/Quest/Effect/QuestEffectBase.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

AQuestObjectiveBase::AQuestObjectiveBase()
{
	ObjectiveArea = CreateDefaultSubobject<UBoxComponent>(TEXT("ObjectiveArea"));
	ObjectiveArea->OnComponentBeginOverlap.AddDynamic(this, &AQuestObjectiveBase::OnBoxBeginOverlap);

	QuestType = EQuestObjectiveType::Destination;
}

void AQuestObjectiveBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor)) { 
		if (auto AbilitySystemComponent = Cast<UAbilitySystemComponent>(AbilitySystemInterface)) {
			// TODO: Replace with actual tag
			// The target is a player character
			if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Player")))) {

				// Create quest effect
				UQuestEffectBase* QuestEffect = NewObject<UQuestEffectBase>();
				QuestEffect->SetQuestEffectTag(QuestObjectiveTag);

				// Create effect spec
				FGameplayEffectSpec EffectSpec;
				EffectSpec.Def = QuestEffect;

				// Apply effect to the player
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(EffectSpec);

				Destroy();
			}
		}
	}
}