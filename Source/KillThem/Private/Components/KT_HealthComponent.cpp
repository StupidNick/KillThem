#include "Components/KT_HealthComponent.h"



UKT_HealthComponent::UKT_HealthComponent()
{
}


void UKT_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;
}


void UKT_HealthComponent::HealthRecovery(const float InHealth)
{
	if (InHealth > 0)
	{
		Health = FMath::Min<float>(MaxHealth, Health + InHealth);
	}
	UE_LOG(LogTemp, Error, TEXT("%f"), Health);
}


void UKT_HealthComponent::DecreaseInHealth(const float InHealth)
{
	if (InHealth < 0)
	{
		Health += InHealth;
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
