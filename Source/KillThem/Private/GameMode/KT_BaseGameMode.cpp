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


void AKT_BaseGameMode::CreateControllers_Implementation()
{
	// for (int32 Counter = 0; Counter <= NumbersOfPlayer; ++Counter)
	// {
	// 	Client
	// }
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


void AKT_BaseGameMode::CreateTeamsInfo_Implementation()
{
	if (!IsValid(GetWorld()) || !HasAuthority()) return;
	
	int32 TeamID = 1;
	for (auto Iteraor = GetWorld()->GetControllerIterator(); Iteraor; ++Iteraor)
	{
		const auto LController = Iteraor->Get();
		if (!IsValid(LController)) continue;

		const auto LPlayerState = Cast<AKT_PlayerState>(LController->PlayerState);
		if (!IsValid(LPlayerState)) continue;

		LPlayerState->SetTeamID(TeamID);
		LPlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		SetPlayerColor(LController);

		TeamID == NumbersOfTeam ? TeamID = 1 : ++TeamID;
	}
}


FLinearColor AKT_BaseGameMode::DetermineColorByTeamID(int32 TeamID) const
{
	if (TeamID - 1 < TeamColors.Num())
	{
		return TeamColors[TeamID - 1];
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