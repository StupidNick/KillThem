#include "Components/KT_HealthComponent.h"

#include "Character/KT_PlayerCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"


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


void UKT_HealthComponent::Initialize(AKT_PlayerCharacter* InCharacter)
{
	PlayerCharacter = InCharacter;
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
	if (Health <= 0)
	{
		OnDead.Broadcast(PlayerCharacter->ControllerOfPlayer);
		IsDead = true;
	}
}


void UKT_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 
	DOREPLIFETIME(UKT_HealthComponent, Health);
	DOREPLIFETIME(UKT_HealthComponent, Shield);
	DOREPLIFETIME(UKT_HealthComponent, IsDead);
}


void UKT_HealthComponent::OnRep_IsDead_Implementation()
{
	PlayerCharacter->DieOnServer();
}


void UKT_HealthComponent::OnRep_Health_Implementation()
{
	OnHPChangeBind.Broadcast(Health);
}


void UKT_HealthComponent::OnRep_Shield_Implementation()
{
	OnSPChangeBind.Broadcast(Shield);
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
		Shield += InShield;
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
			ChangeHealthOnServer(InShield * (1 - ShieldSafeFactor));
		}
		else
		{
			ChangeHealthOnServer(InShield * (1 - ShieldSafeFactor) + Shield);
			Shield = 0;
		}
	}
}


void UKT_HealthComponent::ChangeShieldOnServer_Implementation(const float InShield)
{
	ChangeShield(InShield);
}


void UKT_HealthComponent::Death_Implementation()
{
	IsDead = true;
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	PlayerCharacter->DieOnServer();
}


void UKT_HealthComponent::DeathOnServer_Implementation()
{
	Death();
}