#include "UI/GameModeWD/KT_DMWinWD.h"

#include "Character/Controllers/KT_DeathmatchPlayerController.h"
#include "GameFramework/PlayerState.h"


void UKT_DMWinWD::InitializeWD(TArray<AKT_PlayerState*> PlayerStatesArray, AKT_PlayerController* Controller, const FString& WinnerName) const
{
	FString LWinnerText = WinnerName;
	LWinnerText.Append(PlayerWinText);
	
	if (Controller->PlayerState->GetName().Equals(WinnerName))
	{
		GameOverTextBlock->SetText(FText::FromString(WinText));
	}
	else
	{
		GameOverTextBlock->SetText(FText::FromString(LoseText));
	}
	WinTextBlock->SetText(FText::FromString(LWinnerText));
	
	DeathmatchStatisticTable->InitializeWD(PlayerStatesArray, Cast<AKT_DeathmatchPlayerController>(Controller));
}
