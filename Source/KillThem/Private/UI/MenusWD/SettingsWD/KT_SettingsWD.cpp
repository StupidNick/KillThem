#include "UI/MenusWD/SettingsWD/KT_SettingsWD.h"

#include "Components/Button.h"
#include "GameMode/KT_GameHUD.h"


UKT_SettingsWD::UKT_SettingsWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void UKT_SettingsWD::NativeConstruct()
{
	Super::NativeConstruct();

	if (EscapeButton) EscapeButton->OnClicked.AddDynamic(this, &UKT_SettingsWD::OnEscapeButtonClick);
}


void UKT_SettingsWD::InitializeSettingsWD_Implementation(AKT_GameHUD* InGameHUD)
{
	GameHUD = InGameHUD;
}


void UKT_SettingsWD::OnEscapeButtonClick()
{
	GameHUD->CreateMainMenuWD();
	GameHUD->RemoveSettingsWD();
}