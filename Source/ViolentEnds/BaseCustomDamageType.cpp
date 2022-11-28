#include "BaseCustomDamageType.h"
#include "EntityStats.h"


float UBaseCustomDamageType::ReturnDamageAmount(class UEntityStats* EntityStats, float Damage)
{
    UE_LOG(LogTemp, Warning, TEXT("Base ReturnDamageAmount called, you should not see this."));
    
    return 0.0;
}

void UBaseCustomDamageType::CriticalHit(class UEntityStats* EntityStats, float& CurrentDamageValue)
{
    float RandomNumber = FMath::RandRange(0.0, 100.0);

    if(EntityStats->CriticalHitChance > RandomNumber)
    {
        CurrentDamageValue *= EntityStats->CriticalHitDamageMultiplier;
    }
}