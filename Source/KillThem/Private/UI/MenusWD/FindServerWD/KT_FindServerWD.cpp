#include "UI/MenusWD/FindServerWD/KT_FindServerWD.h"

#include "Components/Button.h"
#include "GameMode/KT_GameHUD.h"


UKT_FindServerWD::UKT_FindServerWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void UKT_FindServerWD::NativeConstruct()
{
	if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UKT_FindServerWD::OnExitButtonClick);
}


void UKT_FindServerWD::InitializeFindServerWD_Implementation(AKT_GameHUD* InGameHUD)
{
	GameHUD = InGameHUD;
}


void UKT_FindServerWD::OnExitButtonClick()
{
	GameHUD->RemoveFindServerWD();
}
