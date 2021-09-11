#include "Components/KT_HealthComponent.h"



UKT_HealthComponent::UKT_HealthComponent()
{
}


void UKT_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* LOwner = GetOwner();
	if (IsValid(LOwner))
	{
		LOwner->OnTakeAnyDamage.AddDynamic(this, &UKT_HealthComponent::TakeDamage);
	}

	Health = DefaultHealth;
}


void UKT_HealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	DecreaseInHealth(Damage);
}


void UKT_HealthComponent::HealthRecovery(const float InHealth)
{
	if (InHealth > 0)
	{
		Health = FMath::Min<float>(MaxHealth, Health + InHealth);
	}
	UE_LOG(LogTemp, Error, TEXT("%f"), Health);
}


void UKT_HealthComponent::DecreaseInHealth(const float InDamage)
{
	if (InDamage > 0)
	{
		Health -= InDamage;
	}
	if (Health < 0)
	{
		IsDead = true;
		Death();
	}
}


void UKT_HealthComponent::ChangeHealthOnServer_Implementation(const bool Add, const float InHealth)
{
	if (Add)
	{
		HealthRecovery(InHealth);
	}
	else
	{
		DecreaseInHealth(InHealth);
	}
}


void UKT_HealthComponent::Death()
{
	//TODO
}

void UKT_HealthComponent::DeathOnServer_Implementation()
{
	Death();
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////Setters//////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////Getters//////////////////////////////////////////////////////////////////////

float UKT_HealthComponent::GetHealth() const
{
	return Health;
}

bool UKT_HealthComponent::GetIsDead() const
{
	return IsDead;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
