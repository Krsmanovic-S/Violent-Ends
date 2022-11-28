#include "PhysicalDamageType.h"
#include "EntityStats.h"


float UPhysicalDamageType::ReturnDamageAmount(class UEntityStats* EntityStats, float Damage)
{
    return Damage - EntityStats->ArmorValue;
}