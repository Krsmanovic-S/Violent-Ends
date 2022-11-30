#include "EnemyAIController.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseEnemy.h"


AEnemyAIController::AEnemyAIController()
{
	this->BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	this->BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	this->AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	this->AIPerceptionComponent->bEditableWhenInherited = true;
	this->AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdatedImpl);
	SetPerceptionComponent(*this->AIPerceptionComponent);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	this->ControlledEnemy = Cast<ABaseEnemy>(InPawn);
	this->EnemyOrigin = GetPawn()->GetActorLocation();

	if(this->BehaviorTree)
	{
		RunBehaviorTree(this->BehaviorTree);

		this->BehaviorTreeComp->StartTree(*this->BehaviorTree);
	}

	if(this->BehaviorTree->BlackboardAsset)
	{
		this->BlackboardComp->InitializeBlackboard(*(this->BehaviorTree->BlackboardAsset));
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(this->bPlayerIsInView)
	{
		this->BlackboardComp->SetValueAsVector(TEXT("PlayerLocation"), this->PlayerCharacter->GetActorLocation());
		this->BlackboardComp->ClearValue(TEXT("LastKnownPlayerLocation"));

		this->SetFocus(this->PlayerCharacter);
	}
	else if(this->BlackboardComp->GetKeyRawData(FName(TEXT("PlayerLocation"))))
	{
		this->BlackboardComp->ClearValue(TEXT("PlayerLocation"));
		this->BlackboardComp->SetValueAsVector(TEXT("LastKnownPlayerLocation"), this->PlayerCharacter->GetActorLocation());
	}
	else
	{
		this->BlackboardComp->ClearValue(TEXT("PlayerLocation"));
		this->BlackboardComp->ClearValue(TEXT("LastKnownPlayerLocation"));

		this->ClearFocus(EAIFocusPriority::Gameplay);
	}

	// This is set to true when the controlled pawns health drops to 0,
	// the trigger for this is in the EntityStats class.
	if(this->bIsDead)
	{
		ControlledEnemy->InitializeDeathTimer();

		this->Destroy();
	}
}

void AEnemyAIController::OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors)
{
    for(auto&& CurrentActor : UpdatedActors)
    {
		if(CurrentActor->IsA<APlayerCharacter>())
		{
			FActorPerceptionBlueprintInfo Info;
        	this->AIPerceptionComponent->GetActorsPerception(CurrentActor, Info);

			if(Info.LastSensedStimuli[0].WasSuccessfullySensed())
			{
				// the actor enters the sense range
				this->bPlayerIsInView = true;
			}
			else
			{
				// the actor leaves the sense range
				this->bPlayerIsInView = false;
			}

			return;
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

	if(!OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}
	
	auto EnemyActor = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	auto PlayerActor = Cast<IGenericTeamAgentInterface>(&Other);

	if(!EnemyActor && !PlayerActor)
	{
		return ETeamAttitude::Neutral;
	}

	FGenericTeamId OtherActorTeamId = NULL;

	if(EnemyActor)
	{
		OtherActorTeamId = EnemyActor->GetGenericTeamId();
	}
	else if(PlayerActor)
	{
		OtherActorTeamId = PlayerActor->GetGenericTeamId();
	}

	FGenericTeamId ThisEntitiesTeamId = this->GetGenericTeamId();

	// 255 is the value for FGenericTeamId::NoTeam, so anything
	// that hasn't got a team will be registered as neutral.
	if(OtherActorTeamId == 255)
	{
		return ETeamAttitude::Neutral;
	}
	// Same ID means that its a friendly enemy.
	else if(OtherActorTeamId == ThisEntitiesTeamId)
	{
		return ETeamAttitude::Friendly;
	}

	return ETeamAttitude::Hostile;
}