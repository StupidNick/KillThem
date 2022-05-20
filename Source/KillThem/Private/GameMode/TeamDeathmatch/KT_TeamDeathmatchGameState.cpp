#include "GameMode/TeamDeathmatch/KT_TeamDeathmatchGameState.h"

#include "GameMode/KT_BaseGameMode.h"


void AKT_TeamDeathmatchGameState::IncrementTeamScore(const int32& TeamID)
{
	switch (TeamID)
	{
		case 0:
			++FirstTeamScore;
			break;
		case 1:
			++SecondTeamScore;
			break;
		default:
			return;
	}
	if (FirstTeamScore >= MyGameMode->WinScore)
	{
		MyGameMode->GameOver("FirstTeam");
	}
	else if (SecondTeamScore >= MyGameMode->WinScore)
	{
		MyGameMode->GameOver("SecondTeam");
	}
}


void AKT_TeamDeathmatchGameState::DecrementTeamScore(const int32& TeamID)
{
	switch (TeamID)
	{
		case 0:
			--FirstTeamScore;
			break;
		case 1:
			--SecondTeamScore;
			break;
		default:
			return;
	}
}


void AKT_TeamDeathmatchGameState::TimeIsOver()
{
	Super::TimeIsOver();

	if (FirstTeamScore == SecondTeamScore)
	{
		MyGameMode->GameOver("Draw");
		return;
	}
	if (FirstTeamScore > SecondTeamScore)
	{
		MyGameMode->GameOver("FirstTeam");
	}
	else
	{
		MyGameMode->GameOver("SecondTeam");
	}
}


void AKT_TeamDeathmatchGameState::BeginPlay()
{
	Super::BeginPlay();
}
