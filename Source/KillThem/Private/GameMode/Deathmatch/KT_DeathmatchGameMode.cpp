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