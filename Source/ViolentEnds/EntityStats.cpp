#include "EntityStats.h"

#include "BaseEnemy.h"
#include "GameplayTagAssetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Shield.h"
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
	if (this->bShouldRegenHealth) { this->RegenerateHealth(); }

	if (this->bShouldRegenStamina) { this->HandleStamina(DeltaTime); }
}

void UEntityStats::DamageTaken(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f)
	{
		LOG_WARNING(LogTemp, "Tried to apply damage of value 0.");
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
		else if (GetOwner()->IsA<AShield>()) { DamagedActor->Destroy(); }
	}
}

void UEntityStats::RegenerateHealth()
{
	this->CurrentHealth += HealthRegenerationAmount * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if (this->CurrentHealth > this->MaximumHealth) { this->CurrentHealth = this->MaximumHealth; }
}

void UEntityStats::HandleStamina(float DeltaTime)
{
	bool bIsMoving = GetOwner()->GetVelocity().Size() > 0;

	if (bIsEntityRunning && bIsMoving) { CurrentStamina -= StaminaDecreaseAmount * DeltaTime; }
	else { CurrentStamina += StaminaRegenerationAmount * DeltaTime; }

	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaximumStamina);

	if (bRunIsToggled && CurrentStamina == 0) { bIsEntityRunning = false; }
}

void UEntityStats::FlatHeal(UPARAM(ref) const float& InputValue)
{
	this->CurrentHealth += InputValue;

	if (this->CurrentHealth > this->MaximumHealth) { this->CurrentHealth = this->MaximumHealth; }
}

void UEntityStats::FlatStaminaAdd(UPARAM(ref) const float& InputValue)
{
	this->CurrentStamina += InputValue;

	if (this->CurrentStamina > this->MaximumStamina) { this->CurrentStamina = this->MaximumStamina; }
}

void UEntityStats::SetMaximumHealth(const float& InputValue)
{
	this->MaximumHealth = InputValue;
	this->OnMaximumHealthUpdated.Broadcast();
}