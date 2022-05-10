#include "UI/MainHUD_WD/KT_StartGameSpectatorWD.h"



void UKT_StartGameSpectatorWD::InitializeWD(AKT_PlayerController* Controller)
{
	MyController = Controller;
	
	FOnInputAction LSpawnDelegate;
	LSpawnDelegate.BindUFunction(this, "SpawnPlayer");
	ListenForInputAction("Interact", IE_Pressed, true, LSpawnDelegate);
}

void UKT_StartGameSpectatorWD::SpawnPlayer()
{
	if (!IsValid(MyController)) return;
	
	StopListeningForInputAction("Interact", IE_Pressed);
	MyController->RespawnPlayerOnClient();
	MyController->RespawnPlayerOnServer();
}
