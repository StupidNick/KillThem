#include "GameMode/Deathmatch/KT_DeathmatchGameMode.h"

#include "Character/Controllers/KT_DeathmatchPlayerController.h"


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
	for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		const auto LController = Cast<AKT_DeathmatchPlayerController>(Iterator->Get());
		if (!IsValid(LController)) continue;

		LController->UpdateStat(FindPlayerStates());
	}
}


void AKT_DeathmatchGameMode::GameOver(const FString& WinnerName)
{
	Super::GameOver(WinnerName);

	const TArray<AKT_PlayerState*> LPlayerStateArray = FindPlayerStates();
	for (auto Iteraor = GetWorld()->GetControllerIterator(); Iteraor; ++Iteraor)
	{
		
		const auto LController = Cast<AKT_DeathmatchPlayerController>(*Iteraor);
		LController->SetPause(true);
		LController->GameOver(LPlayerStateArray, WinnerName);
	}
}
