#include "EnemyQueryContext.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "ViolentEnds/EnemyAIController.h"

void UEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AEnemyAIController* EnemyController =
		Cast<AEnemyAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));

	if (EnemyController)
	{
		// This is going to be used as the Enemy Context.
		AActor* TargetEnemy = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject("SelfActor"));

		if (TargetEnemy) { UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetEnemy); }
	}
}