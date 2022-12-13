// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_TargetInRangeCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_TargetInRangeCheck::UBTDecorator_TargetInRangeCheck() 
{
}

bool UBTDecorator_TargetInRangeCheck::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return false;

	AActor* AIActor = OwnerComp.GetAIOwner()->GetPawn();
	if (!AIActor) return false;

	float AttackRange = Blackboard->GetValueAsFloat(AttackRangeKey.SelectedKeyName);

	float Distance2ActorsSquared = FVector::DistSquared2D(TargetActor->GetActorLocation(), AIActor->GetActorLocation());

	return (FMath::Pow(AttackRange, 2) > Distance2ActorsSquared);

}

