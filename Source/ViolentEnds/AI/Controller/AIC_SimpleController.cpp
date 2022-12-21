#include "AIC_SimpleController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIC_SimpleController::AAIC_SimpleController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	AISight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISight"));

	// Register sight component(s)
	AIPerceptionComponent->ConfigureSense(*AISight);
	AIPerceptionComponent->SetDominantSense(AISight->GetSenseImplementation());

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_SimpleController::OnActorPerceptionUpdated);
	SetPerceptionComponent(*AIPerceptionComponent);
}

void AAIC_SimpleController::OnActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (APawn* AsPawn = Cast<APawn>(Actor))
	{
		// Actor sensed is a pawn

		if (AsPawn->IsPlayerControlled())
		{
			// Actor is player controlled pawn
			if (!GetBlackboardComponent()->GetValueAsObject(FName(TEXT("PlayerActor"))))
			{
				// If the player actor key object is not set then set it
				GetBlackboardComponent()->SetValueAsObject(FName(TEXT("PlayerActor")), Actor);
			}

			// Remove the timer deleting the sensed player
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			if (TimerManager.IsTimerActive(ResetPerceptionTimer))
			{
				TimerManager.ClearTimer(ResetPerceptionTimer);
				UE_LOG(LogTemp, Error, TEXT("Clearing Timer"))
			}
		}

		if (AsPawn->IsPlayerControlled() && !Stimulus.WasSuccessfullySensed())
		{
			// Actor is player controlled pawn
			// Actor is not sensed
			// Remove the key from the PlayerActor blackboard after time

			// Get the timer reset duration
			float ResetTime = GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("ResetTime")));

			// If the key "ResetTime" doesn't exist, default the time to 4.0 seconds
			if ((ResetTime <= 0.f)) { ResetTime = 4.f; }
			UE_LOG(LogTemp, Warning, TEXT("Target is a decaying player signal, null it after %f seconds"), ResetTime)

			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			FTimerDelegate ResetPerceptionDelegate;
			ResetPerceptionDelegate.BindLambda([&] {
				GetBlackboardComponent()->SetValueAsObject(FName(TEXT("PlayerActor")), nullptr);
				UE_LOG(LogTemp, Warning, TEXT("PlayerActor reset for: %s"), *GetFName().ToString())
			});

			if (!TimerManager.IsTimerActive(ResetPerceptionTimer))
			{
				TimerManager.SetTimer(ResetPerceptionTimer, ResetPerceptionDelegate, ResetTime, false);
			}
		}
	}
}

void AAIC_SimpleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		// Try to run the behavior tree
		if (RunBehaviorTree(BehaviorTree))
		{
			// Behavior tree ran successfully
		}
		else { UE_LOG(LogTemp, Error, TEXT("Behavior Tree not set for: %s"), *GetFName().ToString()) }
	}
	else { UE_LOG(LogTemp, Error, TEXT("Behavior Tree not set for: %s"), *GetFName().ToString()) }

	AISight->SightRadius = SightRange;
	AISight->LoseSightRadius = (SightRange + 100.0f);
	AISight->PeripheralVisionAngleDegrees = SightAngle;
	AISight->DetectionByAffiliation.bDetectEnemies = true;
	AISight->DetectionByAffiliation.bDetectNeutrals = true;
	AISight->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*AISight);
	SetPerceptionComponent(*AIPerceptionComponent);

}
