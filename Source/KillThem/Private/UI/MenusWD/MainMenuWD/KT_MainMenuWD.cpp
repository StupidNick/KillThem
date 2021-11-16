#include "UI/MenusWD/MainMenuWD/KT_MainMenuWD.h"

#include "Character/Controllers/KT_PlayerController.h"
#include "Components/Button.h"
#include "GameMode/KT_GameHUD.h"
#include "Kismet/KismetSystemLibrary.h"


UKT_MainMenuWD::UKT_MainMenuWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void UKT_MainMenuWD::NativeConstruct()
{
	if (FindServerButton) FindServerButton->OnClicked.AddDynamic(this, &UKT_MainMenuWD::OnFindServerButtonClick);
	if (SettingsButton) SettingsButton->OnClicked.AddDynamic(this, &UKT_MainMenuWD::OnSettingsButtonClick);
	if (MadeByButton) MadeByButton->OnClicked.AddDynamic(this, &UKT_MainMenuWD::OnMadeByButtonClick);
	if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UKT_MainMenuWD::OnExitButtonClick);
}


void UKT_MainMenuWD::OnFindServerButtonClick()
{
	GameHUD->CreateFindServerWD();
}


void UKT_MainMenuWD::OnSettingsButtonClick()
{
	GameHUD->CreateSettingsWD();
	GameHUD->RemoveMainMenuWD();
}


void UKT_MainMenuWD::OnMadeByButtonClick()
{
	GameHUD->CreateMadeByWD();
}


void UKT_MainMenuWD::OnExitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GameHUD->MyController, EQuitPreference::Quit, true);
}


void UKT_MainMenuWD::InitializeMainMenu_Implementation(AKT_GameHUD* InGameHUD)
{
	GameHUD = InGameHUD;
}
