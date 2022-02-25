#include "Character/Controllers/KT_PlayerController.h"
#include "GameMode/KT_GameHUD.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "GameMode/KT_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"


AKT_PlayerController::AKT_PlayerController()
{
}


void AKT_PlayerController::BeginPlay()
{
	GameHUD = Cast<AKT_GameHUD>(GetHUD());
	PlayerInitialize();
	if (!HasAuthority())
	{
		PlayerCharacter->CreateHUD();
	}
}


void AKT_PlayerController::OnPossess_Implementation(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	PlayerInitialize();
	if (!HasAuthority())
	{
		PlayerCharacter->CreateHUD();
		if (IsValid(PlayerCharacter->ItemsManagerComponent->FirstWeaponSlot) && !HasAuthority())
		{
			PlayerCharacter->ItemsManagerComponent->ChangeAmmoInTheClip(PlayerCharacter->ItemsManagerComponent->FirstWeaponSlot->GetAmmoInTheClip());
			PlayerCharacter->ItemsManagerComponent->ChangeIcon();
		}
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
}


void AKT_PlayerController::RespawnPlayer_Implementation()
{
	FTimerHandle LRespawnTimerHandle;
	FTimerDelegate LRespawnTimerDelegate;

	LRespawnTimerDelegate.BindUFunction(Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(this)), "RespawnPlayer", this);
	GetWorldTimerManager().SetTimer(LRespawnTimerHandle, LRespawnTimerDelegate, Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(this))->TimerForRespawnPlayers, false);
}
