#include "GameMode/KT_GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Character/KT_PlayerCharacter.h"
#include "Character/Controllers/KT_PlayerController.h"
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
		AddFunctionToActiveWDArray("RemoveMainHUD_WD");
	}
}


void AKT_GameHUD::RemoveMainHUD_WD()
{
	if (!IsValid(MainHUD)) return;
	
	MainHUD->RemoveFromParent();
	MainHUD = nullptr;
	RemoveFunctionFromActiveWDArray("RemoveMainHUD_WD");
	
}


void AKT_GameHUD::CreateSniperScopeWD()
{
	if (SniperScopeClass && !IsValid(SniperScope))
	{
		SniperScope = CreateWidget<UKT_SniperScopeWD>(GetWorld(), SniperScopeClass);
		SniperScope->AddToViewport();
		AddFunctionToActiveWDArray("RemoveSniperScopeWD");
	}
}


void AKT_GameHUD::RemoveSniperScopeWD()
{
	if (!IsValid(SniperScope)) return;

	SniperScope->RemoveFromParent();
	SniperScope = nullptr;
	RemoveFunctionFromActiveWDArray("RemoveSniperScopeWD");
}


void AKT_GameHUD::CreateScreenOfDeathWD(AKT_PlayerController* Controller, const FText& InKillerName, const int32& InRespawnTimer)
{
	if (ScreenOfDeathClass && !IsValid(ScreenOfDeath))
	{
		ScreenOfDeath = CreateWidget<UKT_ScreenOfDeathWD>(GetWorld(), ScreenOfDeathClass);
		ScreenOfDeath->AddToViewport();
		ScreenOfDeath->InitializeWD(Controller, InKillerName, InRespawnTimer);
		AddFunctionToActiveWDArray("RemoveScreenOfDeathWD");
	}
}


void AKT_GameHUD::RemoveScreenOfDeathWD()
{
	if (!IsValid(ScreenOfDeath)) return;

	ScreenOfDeath->RemoveFromParent();
	ScreenOfDeath = nullptr;
	RemoveFunctionFromActiveWDArray("RemoveScreenOfDeathWD");
}


void AKT_GameHUD::CreateSpectatorWD()
{
	if (SpectatorWDClass && !IsValid(SpectatorWD))
	{
		SpectatorWD = CreateWidget<UKT_StartGameSpectatorWD>(GetWorld(), SpectatorWDClass);
		SpectatorWD->AddToViewport();
		SpectatorWD->InitializeWD(MyController);
		AddFunctionToActiveWDArray("RemoveSpectatorWD");
	}
}


void AKT_GameHUD::RemoveSpectatorWD()
{
	if (!IsValid(SpectatorWD)) return;

	SpectatorWD->RemoveFromParent();
	SpectatorWD = nullptr;
	RemoveFunctionFromActiveWDArray("RemoveSpectatorWD");
}


void AKT_GameHUD::CreateMainMenuWD()
{
	if (MainMenuClass && !MainMenu)
	{
		MainMenu = CreateWidget<UKT_MainMenuWD>(GetWorld(), MainMenuClass);
		MainMenu->AddToViewport();
		MainMenu->InitializeMainMenu(this);
		AddFunctionToActiveWDArray("RemoveMainMenuWD");
	}
}


void AKT_GameHUD::RemoveMainMenuWD()
{
	if (!IsValid(MainMenu)) return;
	
	MainMenu->RemoveFromParent();
	MainMenu = nullptr;
	RemoveFunctionFromActiveWDArray("RemoveMainMenuWD");
}


void AKT_GameHUD::CreatePauseMenuWD()
{
	if (PauseMenuClass && !PauseMenu)
	{
		PauseMenu = CreateWidget<UKT_PauseMenuWD>(GetWorld(), PauseMenuClass);
		PauseMenu->AddToViewport();
		PauseMenu->InitializeMainMenu(this);
		AddFunctionToActiveWDArray("RemovePauseMenuWD");
	}
}


void AKT_GameHUD::RemovePauseMenuWD()
{
	if (!IsValid(PauseMenu)) return;
	
	PauseMenu->RemoveFromParent();
	PauseMenu = nullptr;

	const FInputModeGameOnly GameOnly;
	MyController->SetInputMode(GameOnly);
	RemoveFunctionFromActiveWDArray("RemovePauseMenuWD");
}


void AKT_GameHUD::CreateFindServerWD()
{
	if (FindServerClass && !FindServer)
	{
		FindServer = CreateWidget<UKT_FindServerWD>(GetWorld(), FindServerClass);
		FindServer->AddToViewport();
		FindServer->InitializeFindServerWD(this);
		AddFunctionToActiveWDArray("RemoveFindServerWD");
	}
}


void AKT_GameHUD::RemoveFindServerWD()
{
	if (!IsValid(FindServer)) return;
	
	FindServer->RemoveFromParent();
	FindServer = nullptr;
	RemoveFunctionFromActiveWDArray("RemoveFindServerWD");
}


void AKT_GameHUD::CreateSettingsWD()
{
	if (SettingsWDClass && !SettingsWD)
	{
		SettingsWD = CreateWidget<UKT_SettingsWD>(GetWorld(), SettingsWDClass);
		SettingsWD->AddToViewport();
		SettingsWD->InitializeSettingsWD(this);
		AddFunctionToActiveWDArray("RemoveSettingsWD");
	}
}


void AKT_GameHUD::RemoveSettingsWD()
{
	if (!IsValid(SettingsWD)) return;
	
	SettingsWD->RemoveFromParent();
	SettingsWD = nullptr;
	RemoveFunctionFromActiveWDArray("RemoveSettingsWD");
}


void AKT_GameHUD::CreateMadeByWD()
{
}


void AKT_GameHUD::RemoveMadeByWD()
{
}


void AKT_GameHUD::AddFunctionToActiveWDArray(FString Name)
{
	ActiveWDNameArr.AddUnique(Name);
}


void AKT_GameHUD::RemoveFunctionFromActiveWDArray(FString Name)
{
	ActiveWDNameArr.RemoveSingle(Name);
}


void AKT_GameHUD::RemoveAllActiveWD()
{
	if (!ActiveWDNameArr.IsValidIndex(0)) return;
	
	for (int32 Iterator = 0; Iterator < ActiveWDNameArr.Num(); ++Iterator)
	{
		//  LDelegate;
		// LDelegate.BindUFunction(this, *Iterator);
		// LDelegate.Execute();
		FOutputDeviceNull OutputDeviceNull;
		CallFunctionByNameWithArguments(*ActiveWDNameArr[Iterator],OutputDeviceNull, nullptr, true);
	}
	ActiveWDNameArr.Empty();
}
