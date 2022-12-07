#include "EntityStats.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"


UEntityStats::UEntityStats()
{
	PrimaryComponentTick.bCanEverTick = true;

	this->bIsEntityRunning = false;
}

void UEntityStats::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UEntityStats::DamageTaken);
}

void UEntityStats::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if(this->bShouldRegenHealth)
	{
		this->HandleHealth();
	}

	if(this->bShouldRegenStamina)
	{
		this->HandleStamina();
	}
}

void UEntityStats::DamageTaken(AActor *DamagedActor, float Damage, const UDamageType *DamageType, AController *Instigator, AActor *DamageCauser)
{
	// Only apply damage if we have any health left.
	if(this->CurrentHealth > 0)
	{
		if(Damage <= 0.f) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to apply damage of value 0."));
			return;
		}

		this->CurrentHealth -= Damage;

		if(this->CurrentHealth <= 0)
		{
			if(DamagedActor->IsA<ABaseEnemy>())
			{
				ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(this->GetOwner());

				EnemyCharacter->InitializeDeathTimer();
			}
		}
	}
}

void UEntityStats::HandleHealth()
{
	this->CurrentHealth += HealthRegenerationAmount * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if(this->CurrentHealth > this->MaximumHealth)
	{
		this->CurrentHealth = this->MaximumHealth;
	}
}

void UEntityStats::HandleStamina()
{
	if(this->bIsEntityRunning && this->CurrentStamina && GetOwner()->GetVelocity().Size() > 0)
	{
		this->CurrentStamina -= this->StaminaDecreaseAmount * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		if(this->CurrentStamina < 0)
		{
			this->CurrentStamina = 0;

			return;
		}
	}
	else if(GetOwner()->GetVelocity().Size() == 0 ||
		   (!this->bIsEntityRunning && this->CurrentStamina < this->MaximumStamina))
	{
		this->CurrentStamina += this->StaminaRegenerationAmount * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		if(this->CurrentStamina >= this->MaximumStamina)
		{
			this->CurrentStamina = this->MaximumStamina;
		}

		return;
	}
}

void UEntityStats::SetMaximumHealth(const float& InputValue)
{
	this->MaximumHealth = InputValue;
	this->OnMaximumHealthUpdated.Broadcast();
}