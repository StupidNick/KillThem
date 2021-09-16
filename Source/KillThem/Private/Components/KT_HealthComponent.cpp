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
	ChangeHealthOnServer(-Damage);
}


void UKT_HealthComponent::ChangeHealth(const float InHealth)
{
	Health = FMath::Min<float>(MaxHealth, Health + InHealth);
	if (Health <= 0)
	{
		Death();
	}
	UE_LOG(LogTemp, Error, TEXT("%s: %f"), *GetOwner()->GetName(), Health);
}


void UKT_HealthComponent::ChangeHealthOnServer_Implementation(const float InHealth)
{
	ChangeHealth(InHealth);
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
