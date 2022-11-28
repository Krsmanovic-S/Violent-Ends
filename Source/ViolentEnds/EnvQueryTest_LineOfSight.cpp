#include "EnvQueryTest_LineOfSight.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"


UEnvQueryTest_LineOfSight::UEnvQueryTest_LineOfSight(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void UEnvQueryTest_LineOfSight::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if(QueryOwner == nullptr)
	{
		return;
	}
	
	TArray<FVector> ContextLocations;

    if(!QueryInstance.PrepareContext(this->Context, ContextLocations))
	{
		return;
	}

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    FCollisionResponseParams ResponseParams;

    for(FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
    
        if(!FPhysicsInterface::RaycastSingle(GetWorld(), HitResult, ItemLocation, ContextLocations[0], ECC_Visibility, QueryParams, 
           ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam))
        {
            It.ForceItemState(EEnvItemStatus::Passed, 1);
        }
        else
        {
            It.ForceItemState(EEnvItemStatus::Passed, 0);
        }
    }
}

FText UEnvQueryTest_LineOfSight::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("Line Of Sight To Context")));
}

FText UEnvQueryTest_LineOfSight::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}