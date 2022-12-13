// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateFacingTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RotateFacingTarget::UBTTask_RotateFacingTarget() 
{}

EBTNodeResult::Type UBTTask_RotateFacingTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	AActor* AIActor = OwnerComp.GetAIOwner()->GetPawn();
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (AIActor && TargetActor) 
	{ 
		FVector CurrentLocation = AIActor->GetActorLocation();
		FVector TargetLocation = TargetActor->GetActorLocation();

		FVector Direction = TargetLocation - CurrentLocation;
		Direction.GetSafeNormal2D();

		FRotator Rotation = FRotationMatrix::MakeFromXZ(Direction, FVector::UpVector).Rotator();

		AAIController* Controller = OwnerComp.GetAIOwner();
		Controller->SetControlRotation(Rotation);

		NodeResult = EBTNodeResult::Succeeded;
	}

	return NodeResult;
}

