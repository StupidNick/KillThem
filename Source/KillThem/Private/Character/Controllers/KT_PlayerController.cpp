#include "Character/Controllers/KT_PlayerController.h"
#include "GameMode/KT_GameHUD.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "GameMode/KT_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AKT_PlayerController::AKT_PlayerController()
{
}


void AKT_PlayerController::BeginPlay()
{
	if (!IsValid(GetWorld())) return;

	GameMode = Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameHUD = Cast<AKT_GameHUD>(GetHUD());
	PlayerInitialize();

	if (!HasAuthority()) return;
	GameMode->GetTeamsInfo(this);
}


void AKT_PlayerController::OnRep_DeathTimerChanged_Implementation()
{
	TimerOfDeath.Broadcast(DeathTimer);
}


void AKT_PlayerController::OnRep_ReadyToRespawn_Implementation()
{
	RespawnReady.Broadcast(ReadyToRespawn);
}


void AKT_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_PlayerController, DeathTimer);
	DOREPLIFETIME(AKT_PlayerController, ReadyToRespawn);
	DOREPLIFETIME(AKT_PlayerController, ArrayOfTeammatesPlayerState);
	DOREPLIFETIME(AKT_PlayerController, ArrayOfEnemiesPlayerState);
}


void AKT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("Escape", IE_Pressed, this, &AKT_PlayerController::OnEscapeButtonPressed);
	InputComponent->BindAction("Statistic", IE_Pressed, this, &AKT_PlayerController::CollectPlayersStates);
	InputComponent->BindAction("Statistic", IE_Released, this, &AKT_PlayerController::HideStatistic);
}


void AKT_PlayerController::OnPossess_Implementation(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	PlayerInitialize();
	if (!IsValid(PlayerCharacter) || !IsValid(GameMode)) return;
	
	if (IsValid(PlayerCharacter->ItemsManagerComponent->GetSelectedWeaponSlot()))
	{
		PlayerCharacter->ItemsManagerComponent->GetSelectedWeaponSlot()->Controller = this;
	}
}


void AKT_PlayerController::PlayerInitialize()
{
	if (IsValid(Cast<AKT_PlayerCharacter>(GetPawn())))
	{
		PlayerCharacter = Cast<AKT_PlayerCharacter>(GetPawn());
		PlayerCharacter->PlayerController = this;
		PlayerCharacter->HUD = GameHUD;
	}
	if (!HasAuthority() && IsValid(PlayerCharacter))
	{
		GameHUD->RespawnPlayer(PlayerCharacter);
	}
}


void AKT_PlayerController::PreparePlayerForRespawnOnServer_Implementation()
{
	DeathTimer = RespawnTime;
	ReadyToRespawn = false;
	
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AKT_PlayerController::CountDownToRespawn, 1, true);
	PreparePlayerForRespawnOnClient();
}


void AKT_PlayerController::PreparePlayerForRespawnOnClient_Implementation()
{
	GameHUD->CreateScreenOfDeathWD(this, FText::FromString("KillerName"), DeathTimer);
}


void AKT_PlayerController::CountDownToRespawn_Implementation()
{
	if (--DeathTimer <= 0)
	{
		GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
		ReadyToRespawn = true;
	}
}


void AKT_PlayerController::RespawnPlayerOnClient_Implementation()
{
	GameHUD->RemoveAllWD();
}


void AKT_PlayerController::RespawnPlayerOnServer_Implementation()
{
	if (DeathTimer > 0 || !IsValid(GameMode)) return;
	if (IsValid(GetPawn()))
	{
		const auto LPawn = GetPawn();
		UnPossess();
		LPawn->Destroy();
	}

	GameMode->RespawnPlayer(this);
}


void AKT_PlayerController::OnEscapeButtonPressed_Implementation()
{
	if (!GameHUD->PauseMenu)
	{
		GameHUD->CreatePauseMenuWD();
	}
	else
	{
		GameHUD->RemovePauseMenuWD();
	}
}


void AKT_PlayerController::CollectPlayersStates_Implementation()
{
	if (!IsValid(GameMode)) return;
	ArrayOfEnemiesPlayerState = GameMode->GetEnemies(this);
	ArrayOfTeammatesPlayerState = GameMode->GetTeammates(this);

	ShowStatistic(ArrayOfTeammatesPlayerState, ArrayOfEnemiesPlayerState);
}


void AKT_PlayerController::ShowStatistic_Implementation(const TArray<AKT_PlayerState*>& TeammatesPlayerStates, const TArray<AKT_PlayerState*>& EnemiesPlayerStates)
{
	if (!IsValid(GameHUD) || HasAuthority()) return;
	
	GameHUD->CreateStatisticTableWD(TeammatesPlayerStates, EnemiesPlayerStates);
}


void AKT_PlayerController::HideStatistic_Implementation()
{
	if (!IsValid(GameHUD)) return;

	GameHUD->RemoveStatisticTableWD();
}


void AKT_PlayerController::UpdateStat_Implementation(const TArray<AKT_PlayerState*>& InArrayOfTeammatesPlayerState,
													const TArray<AKT_PlayerState*>& InArrayOfEnemiesPlayerState)
{
	if (!InArrayOfTeammatesPlayerState.IsValidIndex(0) || !InArrayOfEnemiesPlayerState.IsValidIndex(0)) return;
	
	if (HasAuthority())
	{
		ArrayOfEnemiesPlayerState = InArrayOfTeammatesPlayerState;
		ArrayOfTeammatesPlayerState = InArrayOfEnemiesPlayerState;
	}
	
	StatisticTableUpdate.Broadcast(ArrayOfTeammatesPlayerState, ArrayOfEnemiesPlayerState);
}