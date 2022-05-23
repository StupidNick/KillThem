#include "Character/Controllers/KT_TDMPlayerController.h"

#include "GameMode/KT_GameHUD.h"
#include "GameMode/TeamDeathmatch/KT_TeamDeathmatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"



void AKT_TDMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameMode = Cast<AKT_TeamDeathmatchGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}


void AKT_TDMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("Statistic", IE_Pressed, this, &AKT_TDMPlayerController::CollectPlayersStates);
}


void AKT_TDMPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_TDMPlayerController, ArrayOfTeammatesPlayerState);
	DOREPLIFETIME(AKT_TDMPlayerController, ArrayOfEnemiesPlayerState);
}


void AKT_TDMPlayerController::UpdateStat_Implementation(const TArray<AKT_PlayerState*>& InArrayOfTeammatesPlayerState,
	const TArray<AKT_PlayerState*>& InArrayOfEnemiesPlayerState)
{
	if (!InArrayOfTeammatesPlayerState.IsValidIndex(0) || !InArrayOfEnemiesPlayerState.IsValidIndex(0)) return;
	
	TDMStatisticTableUpdate.Broadcast(InArrayOfTeammatesPlayerState, InArrayOfEnemiesPlayerState);
}


void AKT_TDMPlayerController::CollectPlayersStates_Implementation()
{
	if (!IsValid(CurrentGameMode)) return;
	ArrayOfEnemiesPlayerState = CurrentGameMode->GetEnemies(this);
	ArrayOfTeammatesPlayerState = CurrentGameMode->GetTeammates(this);

	ShowStatistic(ArrayOfTeammatesPlayerState, ArrayOfEnemiesPlayerState);
}


void AKT_TDMPlayerController::ShowStatistic_Implementation(const TArray<AKT_PlayerState*>& TeammatesPlayerStates,
	const TArray<AKT_PlayerState*>& EnemiesPlayerStates)
{
	if (!IsValid(GameHUD) || HasAuthority()) return;
	
	GameHUD->CreateStatisticTableWD(TeammatesPlayerStates, EnemiesPlayerStates);
}


void AKT_TDMPlayerController::HideStatistic()
{
	Super::HideStatistic_Implementation();

	if (!IsValid(GameHUD)) return;

	GameHUD->RemoveTDMStatisticTableWD();
}