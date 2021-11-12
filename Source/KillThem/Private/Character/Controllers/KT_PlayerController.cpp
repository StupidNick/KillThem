#include "Character/Controllers/KT_PlayerController.h"
#include "GameMode/KT_GameHUD.h"

#include "Character/KT_PlayerCharacter.h"


AKT_PlayerController::AKT_PlayerController()
{
}


void AKT_PlayerController::BeginPlay()
{
	
	GameHUD = Cast<AKT_GameHUD>(GetHUD());
	if (Cast<AKT_PlayerCharacter>(GetPawn()))
	{
		PlayerCharacter = Cast<AKT_PlayerCharacter>(GetPawn());
		PlayerCharacter->PlayerController = this;
		PlayerCharacter->HUD = GameHUD;
	}
	if (IsLocalController())
	{
		
	}
}