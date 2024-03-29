#include "GameMode/KT_GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Character/KT_PlayerCharacter.h"
#include "Character/Controllers/KT_DeathmatchPlayerController.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "Character/Controllers/KT_TDMPlayerController.h"
#include "Misc/OutputDeviceNull.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"
#include "UI/MainHUD_WD/KT_StartGameSpectatorWD.h"
#include "UI/menusWD/MainMenuWD/KT_MainMenuWD.h"
#include "UI/MenusWD/MainMenuWD/FindServerWD/KT_FindServerWD.h"
#include "UI/MenusWD/SettingsWD/KT_SettingsWD.h"


AKT_GameHUD::AKT_GameHUD()
{
}


void AKT_GameHUD::BeginPlay()
{
	Super::BeginPlay();

	MyController = Cast<AKT_PlayerController>(GetOwningPlayerController());

	const auto LSpectatorPawn = Cast<AKT_SpectatorPawn>(GetOwningPawn());
	if (IsValid(LSpectatorPawn))
	{
		CreateSpectatorWD();
		return;
	}
	
	MyCharacter = Cast<AKT_PlayerCharacter>(GetOwningPawn());
	if (!IsValid(MyCharacter)) return;
	
	CreateMainHUD_WD();
	MyCharacter->HUD = this;
	// GameInstance = Cast<UPZ_GameInstance>(GetGameInstance());
}


void AKT_GameHUD::DrawHUD()
{
	Super::DrawHUD();
}


void AKT_GameHUD::RespawnPlayer(AKT_PlayerCharacter* InPlayerCharacter)
{
	if (!IsValid(InPlayerCharacter)) return;
	
	MyCharacter = InPlayerCharacter;
	CreateMainHUD_WD();
	MyCharacter->HUD = this;
}


void AKT_GameHUD::CreateMainHUD_WD()
{
	if (MainHUDClass && !IsValid(MainHUD))
	{
		MainHUD = CreateWidget<UKT_MainHUD_WD>(GetWorld(), MainHUDClass);
		MainHUD->AddToViewport();
		MainHUD->InitializeMainHUD(MyCharacter);
	}
}


void AKT_GameHUD::RemoveMainHUD_WD()
{
	if (!IsValid(MainHUD)) return;
	
	MainHUD->RemoveFromParent();
	MainHUD = nullptr;
	
}


void AKT_GameHUD::CreateSniperScopeWD()
{
	if (SniperScopeClass && !IsValid(SniperScope))
	{
		SniperScope = CreateWidget<UKT_SniperScopeWD>(GetWorld(), SniperScopeClass);
		SniperScope->AddToViewport();
	}
}


void AKT_GameHUD::RemoveSniperScopeWD()
{
	if (!IsValid(SniperScope)) return;

	SniperScope->RemoveFromParent();
	SniperScope = nullptr;
}


void AKT_GameHUD::CreateScreenOfDeathWD(AKT_PlayerController* Controller, const FText& InKillerName, const int32& InRespawnTimer)
{
	if (ScreenOfDeathClass && !IsValid(ScreenOfDeath))
	{
		ScreenOfDeath = CreateWidget<UKT_ScreenOfDeathWD>(GetWorld(), ScreenOfDeathClass);
		ScreenOfDeath->AddToViewport();
		ScreenOfDeath->InitializeWD(Controller, InKillerName, InRespawnTimer);
	}
}


void AKT_GameHUD::RemoveScreenOfDeathWD()
{
	if (!IsValid(ScreenOfDeath)) return;

	ScreenOfDeath->RemoveFromParent();
	ScreenOfDeath = nullptr;
}


void AKT_GameHUD::CreateSpectatorWD()
{
	if (SpectatorWDClass && !IsValid(SpectatorWD))
	{
		SpectatorWD = CreateWidget<UKT_StartGameSpectatorWD>(GetWorld(), SpectatorWDClass);
		SpectatorWD->AddToViewport();
		SpectatorWD->InitializeWD(MyController);
	}
}


void AKT_GameHUD::RemoveSpectatorWD()
{
	if (!IsValid(SpectatorWD)) return;

	SpectatorWD->RemoveFromParent();
	SpectatorWD = nullptr;
}


void AKT_GameHUD::CreateStatisticTableWD(TArray<AKT_PlayerState*> TeammatesPlayerStates,
										 TArray<AKT_PlayerState*> EnemiesPlayerStates)
{
	if (TDMStatisticTableWDClass && !IsValid(TDMStatisticTableWD))
	{
		TDMStatisticTableWD = CreateWidget<UKT_TDMStatisticsTableWD>(GetWorld(), TDMStatisticTableWDClass);
		TDMStatisticTableWD->AddToViewport();
		TDMStatisticTableWD->InitializeWD(TeammatesPlayerStates, EnemiesPlayerStates, Cast<AKT_TDMPlayerController>(MyController));
	}
}


void AKT_GameHUD::CreateStatisticTableWD(TArray<AKT_PlayerState*> PlayerStatesArray)
{
	if (DMStatisticTableWDClass && !IsValid(DMStatisticTableWD))
	{
		DMStatisticTableWD = CreateWidget<UKT_DeathmatchStatisticTable>(GetWorld(), DMStatisticTableWDClass);
		DMStatisticTableWD->AddToViewport();
		DMStatisticTableWD->InitializeWD(PlayerStatesArray, Cast<AKT_DeathmatchPlayerController>(MyController));
	}
}


void AKT_GameHUD::RemoveTDMStatisticTableWD()
{
	if (!IsValid(TDMStatisticTableWD)) return;

	TDMStatisticTableWD->RemoveFromParent();
	TDMStatisticTableWD = nullptr;
}


void AKT_GameHUD::RemoveDMStatisticTableWD()
{
	if (!IsValid(DMStatisticTableWD)) return;

	DMStatisticTableWD->RemoveFromParent();
	DMStatisticTableWD = nullptr;
}


void AKT_GameHUD::CreateDMWinWindowWD(TArray<AKT_PlayerState*> PlayerStatesArray, const FString& WinnerName)
{
	if (DMWinWindowWDClass && !IsValid(DMWinWindowWD))
	{
		DMWinWindowWD = CreateWidget<UKT_DMWinWD>(GetWorld(), DMWinWindowWDClass);
		DMWinWindowWD->AddToViewport();
		DMWinWindowWD->InitializeWD(PlayerStatesArray, MyController, WinnerName);
	}
}


void AKT_GameHUD::RemoveDMWinWindowWD()
{
	if (!IsValid(DMWinWindowWD)) return;

	DMWinWindowWD->RemoveFromParent();
	DMWinWindowWD = nullptr;
}


void AKT_GameHUD::CreateMainMenuWD()
{
	if (MainMenuClass && !MainMenu)
	{
		MainMenu = CreateWidget<UKT_MainMenuWD>(GetWorld(), MainMenuClass);
		MainMenu->AddToViewport();
		MainMenu->InitializeMainMenu(this);
	}
}


void AKT_GameHUD::RemoveMainMenuWD()
{
	if (!IsValid(MainMenu)) return;
	
	MainMenu->RemoveFromParent();
	MainMenu = nullptr;
}


void AKT_GameHUD::CreatePauseMenuWD()
{
	if (PauseMenuClass && !PauseMenu)
	{
		PauseMenu = CreateWidget<UKT_PauseMenuWD>(GetWorld(), PauseMenuClass);
		PauseMenu->AddToViewport();
		PauseMenu->InitializeMainMenu(this);
	}
}


void AKT_GameHUD::RemovePauseMenuWD()
{
	if (!IsValid(PauseMenu)) return;
	
	PauseMenu->RemoveFromParent();
	PauseMenu = nullptr;

	const FInputModeGameOnly GameOnly;
	MyController->SetInputMode(GameOnly);
}


void AKT_GameHUD::CreateFindServerWD()
{
	if (FindServerClass && !FindServer)
	{
		FindServer = CreateWidget<UKT_FindServerWD>(GetWorld(), FindServerClass);
		FindServer->AddToViewport();
		FindServer->InitializeFindServerWD(this);
	}
}


void AKT_GameHUD::RemoveFindServerWD()
{
	if (!IsValid(FindServer)) return;
	
	FindServer->RemoveFromParent();
	FindServer = nullptr;
}


void AKT_GameHUD::CreateSettingsWD()
{
	if (SettingsWDClass && !SettingsWD)
	{
		SettingsWD = CreateWidget<UKT_SettingsWD>(GetWorld(), SettingsWDClass);
		SettingsWD->AddToViewport();
		SettingsWD->InitializeSettingsWD(this);
	}
}


void AKT_GameHUD::RemoveSettingsWD()
{
	if (!IsValid(SettingsWD)) return;
	
	SettingsWD->RemoveFromParent();
	SettingsWD = nullptr;
}


void AKT_GameHUD::CreateMadeByWD()
{
}


void AKT_GameHUD::RemoveMadeByWD()
{
}


void AKT_GameHUD::RemoveAllWD()
{
	RemoveMadeByWD();
	RemoveSettingsWD();
	RemoveFindServerWD();
	RemovePauseMenuWD();
	RemoveMainMenuWD();
	RemoveSpectatorWD();
	RemoveScreenOfDeathWD();
	RemoveSniperScopeWD();
	RemoveMainHUD_WD();
	RemoveDMWinWindowWD();
	RemoveTDMStatisticTableWD();
	RemoveDMStatisticTableWD();
}
