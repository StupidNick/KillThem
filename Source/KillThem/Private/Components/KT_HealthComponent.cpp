#include "Components/KT_HealthComponent.h"

#include "Character/KT_PlayerCharacter.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/KT_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"


UKT_HealthComponent::UKT_HealthComponent()
{
}


void UKT_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GetWorld()) || !IsValid(GetOwner())) return;
	
	GameMode = Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerCharacter = Cast<AKT_PlayerCharacter>(GetOwner());
	AActor* LOwner = GetOwner();
	if (IsValid(LOwner))
	{
		LOwner->OnTakeAnyDamage.AddDynamic(this, &UKT_HealthComponent::TakeDamage);
	}

	Health = DefaultHealth;
	Shield = DefaultShield;
}


void UKT_HealthComponent::TakeDamage_Implementation(AActor* DamagedActor, const float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	float LDamage = Damage;
	if (!IsValid(GameMode) || !IsValid(PlayerCharacter) || IsDead) return;
	
	if (GameMode->IsTeammates(PlayerCharacter->Controller, InstigatedBy))
	{
		if (GameMode->DamageToTeammatesAsPercent == 0) return;

		LDamage = Damage * (static_cast<float>(GameMode->DamageToTeammatesAsPercent) / 100);
	}
	
	if (Shield > 0)
	{
		ChangeShieldOnServer(-LDamage);
	}
	else
	{
		ChangeHealthOnServer(-LDamage);
	}
	if (Health <= 0)
	{
		Killed(InstigatedBy);
		OnDead.Broadcast(PlayerCharacter->Controller);
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
	// PlayerCharacter->DieOnServer();
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


void UKT_HealthComponent::Killed_Implementation(AController* KilledController)
{
	if (!IsValid(GameMode)) return;
	IsDead = true;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = IsValid(Player) ? Player->Controller : nullptr;

	GameMode->Killed(KilledController, VictimController);
}