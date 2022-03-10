#include "GameMode/KT_GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Character/KT_PlayerCharacter.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"
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
	MyCharacter = Cast<AKT_PlayerCharacter>(GetOwningPawn());
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
	
	MyCharacter = Cast<AKT_PlayerCharacter>(InPlayerCharacter);
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
	if (MainHUD)
	{
		MainHUD->RemoveFromParent();
		MainHUD = nullptr;
	}
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
	if (MainMenu)
	{
		MainMenu->RemoveFromParent();
		MainMenu = nullptr;
	}
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
	if (PauseMenu)
	{
		PauseMenu->RemoveFromParent();
		PauseMenu = nullptr;

		const FInputModeGameOnly GameOnly;
		MyController->SetInputMode(GameOnly);
	}
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
	if (FindServer)
	{
		FindServer->RemoveFromParent();
		FindServer = nullptr;
	}
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
	if (SettingsWD)
	{
		SettingsWD->RemoveFromParent();
		SettingsWD = nullptr;
	}
}


void AKT_GameHUD::CreateMadeByWD()
{
}


void AKT_GameHUD::RemoveMadeByWD()
{
}
