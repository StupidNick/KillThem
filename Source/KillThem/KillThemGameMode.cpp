// Copyright Epic Games, Inc. All Rights Reserved.

#include "KillThemGameMode.h"
#include "KillThemHUD.h"
#include "KillThemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKillThemGameMode::AKillThemGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AKillThemHUD::StaticClass();
}
