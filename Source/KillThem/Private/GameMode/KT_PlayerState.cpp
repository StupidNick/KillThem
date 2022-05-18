#include "GameMode/KT_PlayerState.h"

#include "GameMode/KT_BaseGameMode.h"
#include "Net/UnrealNetwork.h"



void AKT_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AKT_BaseGameMode>(GetWorld()->GetAuthGameMode());
}


void AKT_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_PlayerState, DeathsNum);
	DOREPLIFETIME(AKT_PlayerState, KillsNum);
	DOREPLIFETIME(AKT_PlayerState, TeamID);
}


void AKT_PlayerState::OnRep_OnKillsNumChanged_Implementation()
{
	if (!IsValid(GameMode)) return;

	GameMode->UpdateStatistic();
}


void AKT_PlayerState::OnRep_OnDeathsNumChanged_Implementation()
{
	if (!IsValid(GameMode)) return;

	GameMode->UpdateStatistic();
}


void AKT_PlayerState::AddDeath_Implementation()
{
	++DeathsNum;
}


void AKT_PlayerState::AddKill_Implementation()
{
	++KillsNum;
}


void AKT_PlayerState::RemoveKill_Implementation()
{
	--KillsNum;
}