#include "GameMode/TeamDeathmatch/KT_TeamDeathmatchGameMode.h"

#include "Character/Controllers/KT_TDMPlayerController.h"
#include "GameMode/TeamDeathmatch/KT_TeamDeathmatchGameState.h"


void AKT_TeamDeathmatchGameMode::Killed(const AController* KilledController, const AController* VictimController)
{
	Super::Killed(KilledController, VictimController);

	const auto KillerPlayerState = Cast<AKT_PlayerState>(KilledController->PlayerState);
	const auto VictimPlayerState = Cast<AKT_PlayerState>(VictimController->PlayerState);
	const auto LGameState = Cast<AKT_TeamDeathmatchGameState>(GameState);
	
	if (KillerPlayerState->GetTeamID() == VictimPlayerState->GetTeamID())
	{
		KillerPlayerState->RemoveKill();
		LGameState->DecrementTeamScore(KillerPlayerState->GetTeamID());
	}
	else
	{
		KillerPlayerState->AddKill();
		LGameState->IncrementTeamScore(KillerPlayerState->GetTeamID());
	}
}


void AKT_TeamDeathmatchGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	const auto LController = Cast<AKT_PlayerController>(NewPlayer);
	SetTeamsInfo(LController);
}


void AKT_TeamDeathmatchGameMode::SetTeamsInfo_Implementation(AKT_PlayerController* Controller)
{
	if (!IsValid(GetWorld()) || !IsValid(Controller)) return;

	const auto LPlayerState = Cast<AKT_PlayerState>(Controller->PlayerState);
	if (!IsValid(LPlayerState)) return;

	LPlayerState->SetTeamID(TeamID);
	// LPlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
	// SetPlayerColor(LController);
	TeamID == NumbersOfTeam - 1 ? TeamID = 0 : ++TeamID;
}


FLinearColor AKT_TeamDeathmatchGameMode::DetermineColorByTeamID(int32 InTeamID) const
{
	if (InTeamID - 1 < TeamColors.Num())
	{
		return TeamColors[InTeamID - 1];
	}
	return DefaultTeamColor;
}


void AKT_TeamDeathmatchGameMode::SetPlayerColor_Implementation(AController* Controller)
{
	if (!IsValid(Controller)) return;

	const auto LCharacter = Cast<AKT_PlayerCharacter>(Controller->GetPawn());
	if (!IsValid(LCharacter)) return;

	const auto LPlayerState = Cast<AKT_PlayerState>(Controller->PlayerState);
	if (!IsValid(LPlayerState)) return;

	LCharacter->SetPlayerColor(LPlayerState->GetTeamColor());
}


void AKT_TeamDeathmatchGameMode::UpdateStatistic()
{
	for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		const auto LController = Cast<AKT_TDMPlayerController>(Iterator->Get());
		if (!IsValid(LController)) continue;

		LController->UpdateStat(GetTeammates(LController), GetEnemies(LController));
	}
}


TArray<AKT_PlayerState*> AKT_TeamDeathmatchGameMode::GetTeammates(const AKT_PlayerController* Controller) const //TODO relocate to child class "Team deathmatch"
{
	TArray<AKT_PlayerState*> LTeammatesArray = FindPlayerStates();
	if (!LTeammatesArray.IsValidIndex(0)) return LTeammatesArray;
	
	const auto LPlayerState = Cast<AKT_PlayerState>(Controller->PlayerState);
	if (!IsValid(LPlayerState)) return LTeammatesArray;

	const int32 LTeamID = LPlayerState->GetTeamID();
	for (int32 PlayerStateIndex = 0; PlayerStateIndex < LTeammatesArray.Num(); ++PlayerStateIndex)
	{
		if (LTeammatesArray[PlayerStateIndex]->GetTeamID() != LTeamID)
		{
			if (!LTeammatesArray.IsValidIndex(PlayerStateIndex)) continue;
			
			LTeammatesArray.RemoveAt(PlayerStateIndex);
		}
	}
	return LTeammatesArray;
}


TArray<AKT_PlayerState*> AKT_TeamDeathmatchGameMode::GetEnemies(const AKT_PlayerController* Controller) const 
{
	TArray<AKT_PlayerState*> LEnemiesArray = FindPlayerStates();
	if (!LEnemiesArray.IsValidIndex(0)) return LEnemiesArray;
	
	const auto LPlayerState = Cast<AKT_PlayerState>(Controller->PlayerState);
	if (!IsValid(LPlayerState)) return LEnemiesArray;

	const int32 LTeamID = LPlayerState->GetTeamID();
	for (int32 PlayerStateIndex = 0; PlayerStateIndex < LEnemiesArray.Num(); ++PlayerStateIndex)
	{
		if (LEnemiesArray[PlayerStateIndex]->GetTeamID() == LTeamID)
		{
			if (!LEnemiesArray.IsValidIndex(PlayerStateIndex)) continue;
			
			LEnemiesArray.RemoveAt(PlayerStateIndex);
		}
	}
	return LEnemiesArray;
}


bool AKT_TeamDeathmatchGameMode::IsTeammates(const AController* Controller1, const AController* Controller2) const
{
	if (!IsValid(Controller1) || !IsValid(Controller2)) return false;

	const auto LPlayerState1 = Cast<AKT_PlayerState>(Controller1->PlayerState);
	const auto LPlayerState2 = Cast<AKT_PlayerState>(Controller2->PlayerState);

	if (LPlayerState1->GetTeamID() == LPlayerState2->GetTeamID()) return true;
	return false;
}