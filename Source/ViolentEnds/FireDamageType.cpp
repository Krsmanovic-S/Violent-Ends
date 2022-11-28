#include "FireDamageType.h"
#include "EntityStats.h"


float UFireDamageType::ReturnDamageAmount(class UEntityStats* EntityStats, float Damage)
{
    return Damage - EntityStats->FireResistance;
}