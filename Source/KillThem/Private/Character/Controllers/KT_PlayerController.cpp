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
	GameHUD = Cast<AKT_GameHUD>(GetHUD());
	PlayerInitialize();
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
}


void AKT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("Escape", IE_Pressed, this, &AKT_PlayerController::OnEscapeButtonPressed);
}


void AKT_PlayerController::OnPossess_Implementation(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	PlayerInitialize();
	if (IsValid(PlayerCharacter->ItemsManagerComponent->GetSelectedWeaponSlot()))
	{
		PlayerCharacter->ItemsManagerComponent->GetSelectedWeaponSlot()->Controller = this;
	}

	if (HasAuthority()) return;
	
	GameHUD->RespawnPlayer(PlayerCharacter);
}


void AKT_PlayerController::PlayerInitialize()
{
	if (IsValid(Cast<AKT_PlayerCharacter>(GetPawn())))
	{
		PlayerCharacter = Cast<AKT_PlayerCharacter>(GetPawn());
		PlayerCharacter->PlayerController = this;
		PlayerCharacter->HUD = GameHUD;
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
	GameHUD->RemoveScreenOfDeathWD();
}


void AKT_PlayerController::RespawnPlayerOnServer_Implementation()
{
	if (DeathTimer > 0) return;

	Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnPlayer(this);
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