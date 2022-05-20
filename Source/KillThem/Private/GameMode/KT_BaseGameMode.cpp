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
	
	StartGame();
	MyGameState = Cast<AKT_BaseGameState>(GameState);
}


void AKT_BaseGameMode::StartGame()
{
}


void AKT_BaseGameMode::GameOver(const FName& WinnerName)
{
	UE_LOG(LogTemp, Error, TEXT("Game over! %s is a winner!"), *WinnerName.ToString());
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
	// SetPlayerColor(Player);
}


void AKT_BaseGameMode::Killed(const AController* KilledController, const AController* VictimController)
{
	const auto KillerPlayerState = Cast<AKT_PlayerState>(KilledController->PlayerState);
	const auto VictimPlayerState = Cast<AKT_PlayerState>(VictimController->PlayerState);

	if (!IsValid(KillerPlayerState) || !IsValid(VictimPlayerState)) return;
	
	VictimPlayerState->AddDeath();
}


TArray<AKT_PlayerState*> AKT_BaseGameMode::FindPlayerStates() const
{
	TArray<AKT_PlayerState*> LPlayerStatesArray;
	for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		AKT_PlayerState* LPlayerState = Cast<AKT_PlayerState>(Iterator->Get()->PlayerState);
		if (!IsValid(LPlayerState)) continue;

		LPlayerStatesArray.AddUnique(LPlayerState);
	}
	return LPlayerStatesArray;
}


bool AKT_BaseGameMode::IsTeammates(const AController* Controller1, const AController* Controller2) const
{
	return false;
}