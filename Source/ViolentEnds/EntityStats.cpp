#include "EntityStats.h"

#include "BaseEnemy.h"
#include "GameplayTagAssetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "ViolentEnds/GlobalTags.h"
#include "ViolentEnds/LogMacros.h"

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

void UEntityStats::TickComponent(
	float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (this->bShouldRegenHealth) { this->HandleHealth(); }

	if (this->bShouldRegenStamina) { this->HandleStamina(); }
}

void UEntityStats::DamageTaken(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	// Only apply damage if we have any health left.
	if (this->CurrentHealth <= 0) { return; }

	if (Damage <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to apply damage of value 0."));
		return;
	}

	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(DamagedActor);
	if (TagInterface && TagInterface->HasMatchingGameplayTag(FGlobalTags::Get().Status_Invincible))
	{
		LOG_WARNING(LogTemp, "Damage not applied due to invincible status");
		return;
	}

	this->CurrentHealth -= Damage;

	if (this->CurrentHealth <= 0)
	{
		if (DamagedActor->IsA<ABaseEnemy>())
		{
			ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(this->GetOwner());

			EnemyCharacter->InitializeDeathTimer();
		}
	}
}

void UEntityStats::HandleHealth()
{
	this->CurrentHealth += HealthRegenerationAmount * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if (this->CurrentHealth > this->MaximumHealth) { this->CurrentHealth = this->MaximumHealth; }
}

void UEntityStats::HandleStamina()
{
	if (this->bIsEntityRunning && this->CurrentStamina && GetOwner()->GetVelocity().Size() > 0)
	{
		this->CurrentStamina -= this->StaminaDecreaseAmount * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		if (this->CurrentStamina < 0) { this->CurrentStamina = 0; }
	}
	else if (GetOwner()->GetVelocity().Size() == 0
			 || (!this->bIsEntityRunning && this->CurrentStamina < this->MaximumStamina))
	{
		this->CurrentStamina += this->StaminaRegenerationAmount * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		if (this->CurrentStamina >= this->MaximumStamina) { this->CurrentStamina = this->MaximumStamina; }
	}
}

void UEntityStats::FlatHeal(UPARAM(ref) const float& InputValue)
{
	this->CurrentHealth += InputValue;

	if(this->CurrentHealth > this->MaximumHealth) { this->CurrentHealth = this->MaximumHealth; }
}

void UEntityStats::FlatStaminaAdd(UPARAM(ref) const float& InputValue)
{
	this->CurrentStamina += InputValue;
	
	if(this->CurrentStamina > this->MaximumStamina) { this->CurrentStamina = this->MaximumStamina; }
}


void UEntityStats::SetMaximumHealth(const float& InputValue)
{
	this->MaximumHealth = InputValue;
	this->OnMaximumHealthUpdated.Broadcast();
}