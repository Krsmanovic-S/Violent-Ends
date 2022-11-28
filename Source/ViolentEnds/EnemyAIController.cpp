#include "EnemyAIController.h"
#include "GameFramework/Pawn.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseEnemy.h"
#include "Perception/AISense_Sight.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


AEnemyAIController::AEnemyAIController()
{
	this->AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PawnSensingComponent"));
	
	this->BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));

	this->BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	SetGenericTeamId(FGenericTeamId(1));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	this->ControlledEnemy = Cast<ABaseEnemy>(InPawn);

	this->EnemyOrigin = GetPawn()->GetActorLocation();

	if(this->ControlledEnemy && this->ControlledEnemy->EnemyBehaviorTree)
	{
		this->BehaviorTree = this->ControlledEnemy->EnemyBehaviorTree;

		RunBehaviorTree(this->BehaviorTree);

		this->BehaviorTreeComp->StartTree(*this->BehaviorTree);
	}

	if(this->BlackboardComp != NULL && this->BehaviorTree != NULL)
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