#include "Medkit.h"
#include "PlayerCharacter.h"
#include "EntityStats.h"


UMedkit::UMedkit()
{
    this->ItemDisplayName = FText::FromString("Basic Medkit");
    this->ItemDescription = FText::FromString("Heals for 20 HP.");
}

void UMedkit::Use(class APlayerCharacter* Player)
{ 
    UE_LOG(LogTemp, Warning, TEXT("You used a medkit."));

    if(this->FlatHealAmount != 0 && Player->PlayerStats->CurrentHealth != Player->PlayerStats->MaximumHealth)
    {
        Player->PlayerStats->CurrentHealth += this->FlatHealAmount;

        if(Player->PlayerStats->CurrentHealth > Player->PlayerStats->MaximumHealth)
        {
            Player->PlayerStats->CurrentHealth = Player->PlayerStats->MaximumHealth;
        }
    }
}
