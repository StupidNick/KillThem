#include "GameMode/Deathmatch/KT_DeathmatchGameMode.h"

#include "Character/Controllers/KT_DeathmatchPlayerController.h"
#include "GameMode/Deathmatch/KT_DeathmatchGameState.h"


void AKT_DeathmatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameState = Cast<AKT_DeathmatchGameState>(GameState);
}


void AKT_DeathmatchGameMode::Killed(const AController* KilledController, const AController* VictimController)
{
	Super::Killed(KilledController, VictimController);

	const auto KillerPlayerState = Cast<AKT_PlayerState>(KilledController->PlayerState);
	
	if (KilledController == VictimController)
	{
		KillerPlayerState->RemoveKill();
	}
	else
	{
		KillerPlayerState->AddKill();
		if (KillerPlayerState->GetKills() >= WinScore)
		{
			GameOver(*KillerPlayerState->GetName());
		}
	}
}


void AKT_DeathmatchGameMode::UpdateStatistic()
{
	for (const auto Controller: CurrentGameState->PlayerControllers)
	{
		if (!IsValid(Controller)) continue;

		Controller->UpdateStat(FindPlayerStates());
	}
}


void AKT_DeathmatchGameMode::GameOver(const FString& WinnerName)
{
	Super::GameOver(WinnerName);

	const TArray<AKT_PlayerState*> LPlayerStateArray = FindPlayerStates();
	for (const auto Controller : CurrentGameState->PlayerControllers)
	{
		Controller->SetPause(true);
		Controller->GameOver(LPlayerStateArray, WinnerName);
	}
}


void AKT_DeathmatchGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	CurrentGameState->PlayerControllers.AddUnique(Cast<AKT_DeathmatchPlayerController>(NewPlayer));
}
