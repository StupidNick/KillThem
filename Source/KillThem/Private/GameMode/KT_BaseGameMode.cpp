#include "GameMode/KT_BaseGameMode.h"

#include "EngineUtils.h"
#include "Character/KT_PlayerCharacter.h"
#include "GameMode/KT_PlayerState.h"



AKT_BaseGameMode::AKT_BaseGameMode()
{
}


void AKT_BaseGameMode::StartPlay()
{
	Super::StartPlay();
	
	GameTimer = GameTime;
	StartGame();
}


bool AKT_BaseGameMode::IsTeammates(const AController* Controller1, const AController* Controller2) const
{
	if (!IsValid(Controller1) || !IsValid(Controller2)) return false;

	const auto LPlayerState1 = Cast<AKT_PlayerState>(Controller1->PlayerState);
	const auto LPlayerState2 = Cast<AKT_PlayerState>(Controller2->PlayerState);

	if (LPlayerState1->GetTeamID() == LPlayerState2->GetTeamID()) return true;
	return false;
}


void AKT_BaseGameMode::StartGame()
{
	GameTimerUpdate();
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AKT_BaseGameMode::GameTimerUpdate, 1.0f, true);
}


void AKT_BaseGameMode::GameTimerUpdate()
{
	UE_LOG(LogTemp, Error, TEXT("Time: %i"), GameTimer);
	
	if (--GameTimer <= 0)
	{
		GetWorldTimerManager().ClearTimer(GameTimerHandle);
		GameOver();
	}
}


void AKT_BaseGameMode::GameOver()
{
	//TODO create widget game over
	// for (auto Iteraor = GetWorld()->GetControllerIterator(); Iteraor; ++Iteraor)
	// {
	// 	Cast<AKT_PlayerController>(*Iteraor)->SetPause(true);
	// }
}


void AKT_BaseGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_BaseGameMode::RespawnPlayer_Implementation(AController* Player)
{
	if (!IsValid(Player) || !HasAuthority()) return;
	
	const FTransform LSpawnTransform = ChoosePlayerStart(Player)->GetActorTransform();
	const FActorSpawnParameters LSpawnInfo;
	AKT_PlayerCharacter* LPawn = GetWorld()->SpawnActor<AKT_PlayerCharacter>(DefaultCharacterClass, LSpawnTransform.GetLocation(), LSpawnTransform.GetRotation().Rotator(), LSpawnInfo);

	if (IsValid(LPawn))
	{
		Player->Possess(LPawn);
	}
	SetPlayerColor(Player);
}


void AKT_BaseGameMode::GetTeamsInfo_Implementation(AKT_PlayerController* Controller)
{
	if (!IsValid(GetWorld()) || !IsValid(Controller)) return;

	const auto LPlayerState = Cast<AKT_PlayerState>(Controller->PlayerState);
	if (!IsValid(LPlayerState)) return;

	LPlayerState->SetTeamID(TeamID);
	// LPlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
	// SetPlayerColor(LController);
	TeamID == NumbersOfTeam ? TeamID = 1 : ++TeamID;
}


FLinearColor AKT_BaseGameMode::DetermineColorByTeamID(int32 inTeamID) const
{
	if (inTeamID - 1 < TeamColors.Num())
	{
		return TeamColors[inTeamID - 1];
	}
	return DefaultTeamColor;
}


void AKT_BaseGameMode::SetPlayerColor_Implementation(AController* Controller)
{
	if (!IsValid(Controller)) return;

	const auto LCharacter = Cast<AKT_PlayerCharacter>(Controller->GetPawn());
	if (!IsValid(LCharacter)) return;

	const auto LPlayerState = Cast<AKT_PlayerState>(Controller->PlayerState);
	if (!IsValid(LPlayerState)) return;

	LCharacter->SetPlayerColor(LPlayerState->GetTeamColor());
}


void AKT_BaseGameMode::Killed(const AController* KilledController, const AController* VictimController) const
{
	const auto KillerPlayerState = Cast<AKT_PlayerState>(KilledController->PlayerState);
	const auto VictimPlayerState = Cast<AKT_PlayerState>(VictimController->PlayerState);

	if (!IsValid(KillerPlayerState) || !IsValid(VictimPlayerState)) return;
	
	if (KillerPlayerState->GetTeamID() == VictimPlayerState->GetTeamID())
	{
		KillerPlayerState->RemoveKill();
	}
	else
	{
		KillerPlayerState->AddKill();
	}
	VictimPlayerState->AddDeath();
}


void AKT_BaseGameMode::UpdateStatistic() //TODO relocate to child class "Team deathmatch"
{
	for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		const auto LController = Cast<AKT_PlayerController>(Iterator->Get());
		if (!IsValid(LController)) continue;

		LController->UpdateStat(GetTeammates(LController), GetEnemies(LController));
	}
}


TArray<AKT_PlayerState*> AKT_BaseGameMode::GetTeammates(const AKT_PlayerController* Controller) const //TODO relocate to child class "Team deathmatch"
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


TArray<AKT_PlayerState*> AKT_BaseGameMode::GetEnemies(const AKT_PlayerController* Controller) const 
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


TArray<AKT_PlayerState*> AKT_BaseGameMode::FindPlayerStates() const
{
	TArray<AKT_PlayerState*> LTeammateArray;
	for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		AKT_PlayerState* LPlayerState = Cast<AKT_PlayerState>(Iterator->Get()->PlayerState);
		if (!IsValid(LPlayerState)) continue;

		LTeammateArray.AddUnique(LPlayerState);
	}
	return LTeammateArray;
}
