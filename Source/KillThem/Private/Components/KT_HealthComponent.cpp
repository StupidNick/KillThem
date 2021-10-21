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
	Shield = DefaultShield;
}


void UKT_HealthComponent::TakeDamage(AActor* DamagedActor, const float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Shield > 0)
	{
		ChangeShieldOnServer(-Damage);
	}
	else
	{
		ChangeHealthOnServer(-Damage);
	}
}


void UKT_HealthComponent::ChangeHealth(const float InHealth)
{
	Health = FMath::Min<float>(MaxHealth, Health + InHealth);
	if (Health <= 0)
	{
		Death();
	}
	UE_LOG(LogTemp, Error, TEXT("%s: %f(Health), %f(Shield)"), *GetOwner()->GetName(), Health, Shield);
}


void UKT_HealthComponent::ChangeHealthOnServer_Implementation(const float InHealth)
{
	ChangeHealth(InHealth);
}


void UKT_HealthComponent::ChangeShield(const float InShield)
{
	if (InShield > 0)
	{
		Shield -= InShield;
		if (Shield > MaxHealth)
		{
			Shield = MaxHealth;
		}
	}
	else
	{
		Shield += InShield * ShieldSafeFactor;
		if (Shield > 0)
		{
			ChangeHealth(InShield * (1 - ShieldSafeFactor));
		}
		else
		{
			ChangeHealth(InShield * (1 - ShieldSafeFactor) + Shield);
			Shield = 0;
		}
	}
}


void UKT_HealthComponent::ChangeShieldOnServer_Implementation(const float InShield)
{
	ChangeShield(InShield);
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
