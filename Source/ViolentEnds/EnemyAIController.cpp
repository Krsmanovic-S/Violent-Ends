#include "EnemyAIController.h"

#include "BaseEnemy.h"
#include "LogMacros.h"
#include "Engine/GameEngine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	InitAIPerceptionSenses();
}

void AEnemyAIController::OnUnPossess()
{
	this->Destroy();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	this->ControlledEnemy = Cast<ABaseEnemy>(InPawn);

	this->EnemyOrigin = InPawn->GetActorLocation();

	if (this->BehaviorTree)
	{
		RunBehaviorTree(this->BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Behavior Tree Set."));
		return;
	}

	if (this->BehaviorTree->BlackboardAsset)
	{
		this->GetBlackboardComponent()->InitializeBlackboard(*(this->BehaviorTree->BlackboardAsset));

		this->GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), this->ControlledEnemy);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// TSubclassOf<ABaseEnemy> EnemyClass;
	// UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), EnemyClass, this->AllEnemies);
}

void AEnemyAIController::OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors)
{
	for (auto&& CurrentActor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		PerceptionComponent->GetActorsPerception(CurrentActor, Info);
		for (auto Sense : Info.LastSensedStimuli)
		{
			const bool NewlySensed = Sense.WasSuccessfullySensed();
			if (Sense.Type == DamageID || Sense.Type == HearID)
			{
				if (NewlySensed && EnemyState < EAIEnemyState::VisualPursuit)
				{
					ClearFocus(EAIFocusPriority::Gameplay);
					GetBlackboardComponent()->SetValueAsVector(TEXT("LocationInterest"), CurrentActor->GetActorLocation());
					EnemyState = EAIEnemyState::AlertedToLocation;
					GetBlackboardComponent()->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(EAIEnemyState::AlertedToLocation));
				}
			}
			else if (Sense.Type ==  SightID)
			{
				if (NewlySensed)
				{
					PlayerCharacter = static_cast<APlayerCharacter*>(CurrentActor);
					GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerActor"), CurrentActor);
					SetFocus(CurrentActor);
					EnemyState = EAIEnemyState::VisualPursuit;
					GetBlackboardComponent()->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(EAIEnemyState::VisualPursuit));
				}
				else
				{
					PlayerCharacter = nullptr;
					GetBlackboardComponent()->ClearValue(TEXT("PlayerActor"));
					ClearFocus(EAIFocusPriority::Gameplay);
					GetBlackboardComponent()->SetValueAsVector(TEXT("LocationInterest"), CurrentActor->GetActorLocation());
					EnemyState = EAIEnemyState::LostVisualPursuit;
					GetBlackboardComponent()->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(EAIEnemyState::LostVisualPursuit));
				}
			}
		}
	}
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
	return FGenericTeamId(1);
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);

	if (!OtherPawn) { return ETeamAttitude::Neutral; }

	auto EnemyActor = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	auto PlayerActor = Cast<IGenericTeamAgentInterface>(&Other);

	if (!EnemyActor && !PlayerActor) { return ETeamAttitude::Neutral; }

	FGenericTeamId OtherActorTeamId = NULL;

	if (EnemyActor) { OtherActorTeamId = EnemyActor->GetGenericTeamId(); }
	else if (PlayerActor) { OtherActorTeamId = PlayerActor->GetGenericTeamId(); }

	FGenericTeamId ThisEntitiesTeamId = this->GetGenericTeamId();

	// 255 is the value for FGenericTeamId::NoTeam, so anything
	// that hasn't got a team will be registered as neutral.
	if (OtherActorTeamId == 255) { return ETeamAttitude::Neutral; }
	// Same ID means that its a friendly enemy.
	else if (OtherActorTeamId == ThisEntitiesTeamId) { return ETeamAttitude::Friendly; }

	return ETeamAttitude::Hostile;
}

// Consolidate the setup of the perception system for this AI Controller in a method
void AEnemyAIController::InitAIPerceptionSenses()
{
	UAIPerceptionComponent* AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	AIPerceptionComponent->bEditableWhenInherited = true;

	//Create a Damage Sense
	UAISenseConfig_Damage* Config_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(FName("Damage Config"));

	//Create a Sight Sense
	UAISenseConfig_Sight* Config_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
	Config_Sight->SightRadius = 1000.f;
	Config_Sight->LoseSightRadius = 1500.f;
	Config_Sight->PeripheralVisionAngleDegrees = 90.f;
	Config_Sight->DetectionByAffiliation.bDetectEnemies = true;

	UAISenseConfig_Hearing* Config_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(FName("Hearing Config"));
	Config_Hearing->HearingRange = 1500.f;
	Config_Hearing->DetectionByAffiliation.bDetectEnemies = true;

	//Register the senses in order to our Perception Component
	AIPerceptionComponent->ConfigureSense(*Config_Damage);
	AIPerceptionComponent->ConfigureSense(*Config_Sight);
	AIPerceptionComponent->ConfigureSense(*Config_Hearing);
	
	SetPerceptionComponent(*AIPerceptionComponent);
}