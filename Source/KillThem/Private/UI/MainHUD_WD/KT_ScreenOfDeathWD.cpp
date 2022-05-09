#include "UI/MainHUD_WD/KT_ScreenOfDeathWD.h"

#include "Components/TextBlock.h"
#include "GameMode/KT_GameHUD.h"


void UKT_ScreenOfDeathWD::InitializeWD(AKT_PlayerController* Controller, const FText& InKillerName, const int32& InRespawnTimer)
{
	MyController = Controller;
	Controller->TimerOfDeath.AddDynamic(this, &UKT_ScreenOfDeathWD::UpdateWD);
	Controller->RespawnReady.AddDynamic(this, &UKT_ScreenOfDeathWD::SetReadyRespawn);
	
	KillerName->SetText(InKillerName);
	RespawnTimer->SetText(FText::AsNumber(InRespawnTimer));
}


void UKT_ScreenOfDeathWD::UpdateWD(const int32& InRespawnTimer)
{
	RespawnTimer->SetText(FText::AsNumber(InRespawnTimer));
}


void UKT_ScreenOfDeathWD::SetReadyRespawn(const bool CanRespawn)
{
	if (!IsValid(MyController) || !CanRespawn) return;
	
	RespawnTimer->SetText(ReadyToRespawnText);

	FOnInputAction LRespawnDelegate;
	LRespawnDelegate.BindUFunction(this, "RespawnPlayer");
	ListenForInputAction("Interact", IE_Pressed, true, LRespawnDelegate);
}


void UKT_ScreenOfDeathWD::RespawnPlayer()
{
	MyController->RespawnPlayerOnClient();
	MyController->RespawnPlayerOnServer();
}


