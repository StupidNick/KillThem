#include "GameMode/KT_GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"


AKT_GameHUD::AKT_GameHUD()
{
}


void AKT_GameHUD::BeginPlay()
{
	Super::BeginPlay();

	MyController = Cast<AKT_PlayerController>(GetOwningPlayerController());
	// GameInstance = Cast<UPZ_GameInstance>(GetGameInstance());
}


void AKT_GameHUD::DrawHUD()
{
	Super::DrawHUD();
}


void AKT_GameHUD::CreateMainHUD()
{
	if (MainHUDClass && !MainHUD)
	{
		MainHUD = CreateWidget<UKT_MainHUD_WD>(GetWorld(), MainHUDClass);
		MainHUD->AddToViewport();
		MainHUD->InitializeMainHUD(MyController->PlayerCharacter);
	}
}


void AKT_GameHUD::RemoveMainHUD()
{
	if (MainHUD)
	{
		MainHUD->RemoveFromParent();
		MainHUD = nullptr;
	}
}
