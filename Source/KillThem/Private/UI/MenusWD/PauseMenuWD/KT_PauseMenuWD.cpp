#include "UI/MenusWD/PauseMenuWD/KT_PauseMenuWD.h"

#include "Character/Controllers/KT_PlayerController.h"
#include "Components/Button.h"
#include "Input/Reply.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/KT_GameHUD.h"

#include "GameFramework/PlayerInput.h"
#include "GameFramework/InputSettings.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"


UKT_PauseMenuWD::UKT_PauseMenuWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}


FReply UKT_PauseMenuWD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray <FInputActionKeyMapping> LOutMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(FName("Escape"), LOutMappings); 

	if (InKeyEvent.GetKey() == LOutMappings[0].Key)
	{
		GameHUD->RemovePauseMenuWD();
		Reply = FReply::Handled();
	}
	return Reply;
}


void UKT_PauseMenuWD::NativeConstruct()
{
	if (ResumeButton) ResumeButton->OnClicked.AddDynamic(this, &UKT_PauseMenuWD::OnResumeButtonClick);
	if (SettingsButton) SettingsButton->OnClicked.AddDynamic(this, &UKT_PauseMenuWD::OnSettingsButtonClick);
	if (ExitToMenuButton) ExitToMenuButton->OnClicked.AddDynamic(this, &UKT_PauseMenuWD::OnExitToMenuButtonClick);
	if (ExitGameButton) ExitGameButton->OnClicked.AddDynamic(this, &UKT_PauseMenuWD::OnExitGameButtonClick);
}


void UKT_PauseMenuWD::OnResumeButtonClick()
{
	GameHUD->RemovePauseMenuWD();
}


void UKT_PauseMenuWD::OnSettingsButtonClick()
{
	//TODO
}


void UKT_PauseMenuWD::OnExitToMenuButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}


void UKT_PauseMenuWD::OnExitGameButtonClick()
{
	// GameHUD->MyController->ConsoleCommand("quit");
	UKismetSystemLibrary::QuitGame(GetWorld(), GameHUD->MyController, EQuitPreference::Quit, true);
}


void UKT_PauseMenuWD::InitializeMainMenu_Implementation(AKT_GameHUD* InGameHUD)
{
	GameHUD = InGameHUD;

	const FInputModeUIOnly UIOnly;
	GameHUD->MyController->SetInputMode(UIOnly);
	
	SetFocus();
}

