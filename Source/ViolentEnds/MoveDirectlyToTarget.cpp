// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveDirectlyToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveDirectlyToTarget::UBTTask_MoveDirectlyToTarget()
{
	NodeName = "MoveDirectlyToTarget";


}

EBTNodeResult::Type UBTTask_MoveDirectlyToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Controller = OwnerComp.GetAIOwner();
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* DestinationActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	FVector Location = Blackboard->GetValueAsVector(TargetLocationKey.SelectedKeyName);
	float Radius = Blackboard->GetValueAsFloat(RadiusKey.SelectedKeyName);

	Radius *= .75f;

	FVector Destination = DestinationActor ? DestinationActor->GetActorLocation() : Location;

	auto PathResult = Controller->MoveToLocation(Destination, Radius);

	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	switch (PathResult)
	{
		case EPathFollowingRequestResult::Failed:
			break;
		case EPathFollowingRequestResult::AlreadyAtGoal:
			NodeResult = EBTNodeResult::Succeeded;
			break;
		case EPathFollowingRequestResult::RequestSuccessful:
			NodeResult = EBTNodeResult::Succeeded;
			break;
		default:
			break;
	}
	return NodeResult;
}

EBTNodeResult::Type UBTTask_MoveDirectlyToTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
