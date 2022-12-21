#include "FindStrafeLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ViolentEnds/EnemyAIController.h"

UFindStrafeLocation::UFindStrafeLocation()
{
	bCreateNodeInstance = true;
	NodeName = "Find Strafe Location";
}

EBTNodeResult::Type UFindStrafeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	this->EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (this->EnemyController && this->LocationSeekerQuery)
	{
		// Call the query to execute and process the results using our own custom function.
		LocationSeekerQueryRequest = FEnvQueryRequest(this->LocationSeekerQuery, this->EnemyController);
		LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UFindStrafeLocation::OnQueryFinished);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UFindStrafeLocation::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	int32 Index = 0;
	float CurrentBestScore = 0;

	// Store all EQS results as locations.
	TArray<FVector> LocationArray;
	Result->GetAllAsLocations(LocationArray);

	for (auto& CurrentLocation : LocationArray)
	{
		// Set the StrafeLocation if the distance check goes through and we have the highest score.
		if (IsDistanceGreater(CurrentLocation) && Result->GetItemScore(Index) > CurrentBestScore)
		{
			this->StrafeLocation = CurrentLocation;
			CurrentBestScore = Result->GetItemScore(Index);
		}

		Index++;
	}

	this->EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), this->StrafeLocation);
}

bool UFindStrafeLocation::IsDistanceGreater(FVector Location)
{
	// if (this->EnemyController->AllEnemies.Num() <= 1) { return true; }

	bool ConsiderLocation = true;
	float CalculatedDistance = 0;

	// Check if the distance between the passed in EQS Location
	// is less than the distance parameter of the query.
	// for (auto& EnemyActor : this->EnemyController->AllEnemies)
	// {
	// 	CalculatedDistance = FVector::Dist(Location, EnemyActor->GetActorLocation());
	//
	// 	if (CalculatedDistance <= this->MaximalDistanceBetweenEnemies)
	// 	{
	// 		ConsiderLocation = false;
	// 		break;
	// 	}
	// }

	return ConsiderLocation;
}