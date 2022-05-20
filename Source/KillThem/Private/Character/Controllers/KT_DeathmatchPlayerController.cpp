#include "Character/Controllers/KT_DeathmatchPlayerController.h"

#include "GameMode/KT_GameHUD.h"
#include "GameMode/Deathmatch/KT_DeathmatchGameMode.h"
#include "Kismet/GameplayStatics.h"


void AKT_DeathmatchPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameMode = Cast<AKT_DeathmatchGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}


void AKT_DeathmatchPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("Statistic", IE_Pressed, this, &AKT_DeathmatchPlayerController::CollectPlayersStates);
}


void AKT_DeathmatchPlayerController::UpdateStat_Implementation(const TArray<AKT_PlayerState*>& PlayerStatesArray)
{
	if (!PlayerStatesArray.IsValidIndex(0)) return;
	
	DMStatisticTableUpdate.Broadcast(PlayerStatesArray);
}


void AKT_DeathmatchPlayerController::CollectPlayersStates_Implementation()
{
	if (!IsValid(CurrentGameMode)) return;

	ShowStatistic(CurrentGameMode->FindPlayerStates());
}


void AKT_DeathmatchPlayerController::ShowStatistic_Implementation(const TArray<AKT_PlayerState*>& PlayerStatesArray)
{
	if (!IsValid(GameHUD) || HasAuthority()) return;
	
	GameHUD->CreateStatisticTableWD(PlayerStatesArray);
}


void AKT_DeathmatchPlayerController::HideStatistic()
{
	Super::HideStatistic();

	if (!IsValid(GameHUD) || HasAuthority()) return;
	
	GameHUD->RemoveDMStatisticTableWD();
}