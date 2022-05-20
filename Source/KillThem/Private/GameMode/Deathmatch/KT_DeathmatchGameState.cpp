#include "GameMode/Deathmatch/KT_DeathmatchGameState.h"

#include "GameMode/Deathmatch/KT_DeathmatchGameMode.h"


void AKT_DeathmatchGameState::TimeIsOver()
{
	Super::TimeIsOver();

	const auto LGameMode = Cast<AKT_DeathmatchGameMode>(GetWorld()->GetAuthGameMode());

	const auto LPlayerStatesArray = LGameMode->FindPlayerStates();
	if (!LPlayerStatesArray.IsValidIndex(0)) return;
	
	AKT_PlayerState* LWinnerPlayerState = LPlayerStatesArray[0];
	for (const auto PlayerState : LPlayerStatesArray)
	{
		if (PlayerState->GetKills() > LWinnerPlayerState->GetKills())
		{
			LWinnerPlayerState = PlayerState;
			continue;
		}
		if (PlayerState->GetKills() != LWinnerPlayerState->GetKills()) continue;
		
		if (PlayerState->GetDeaths() < LWinnerPlayerState->GetDeaths())
		{
			LWinnerPlayerState = PlayerState;
		}
	}

	LGameMode->GameOver(*LWinnerPlayerState->GetName());
}
