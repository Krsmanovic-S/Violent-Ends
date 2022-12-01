#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerCharacter.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyAIController::AEnemyAIController()
{
	this->BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	this->AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	this->AIPerceptionComponent->bEditableWhenInherited = true;
	this->AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdatedImpl);
	SetPerceptionComponent(*this->AIPerceptionComponent);
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

	if(this->BehaviorTree)
	{
		RunBehaviorTree(this->BehaviorTree);

		this->BehaviorTreeComp->StartTree(*this->BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Behavior Tree Set."));
		return;
	}

	if(this->BehaviorTree->BlackboardAsset)
	{
		this->GetBlackboardComponent()->InitializeBlackboard(*(this->BehaviorTree->BlackboardAsset));

		this->GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), this->ControlledEnemy);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	this->GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerActor"), this->PlayerCharacter);

	TSubclassOf<ABaseEnemy> EnemyClass;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), EnemyClass, this->AllEnemies);
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
				// Sensed actor enters the sense range
				this->GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), this->PlayerCharacter->GetActorLocation());
				this->SetFocus(this->PlayerCharacter);
				this->ControlledEnemy->GetCharacterMovement()->MaxWalkSpeed = this->ControlledEnemy->RunMovementSpeed;
			}
			else
			{
				// Sensed actor leaves the sense range
				UE_LOG(LogTemp, Warning, TEXT("Player Left Focus."));

				this->GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
				this->GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), this->PlayerCharacter->GetActorLocation());
				this->ClearFocus(EAIFocusPriority::Gameplay);
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