#include "InteractiveObjects/UsableObjects/KT_BaseBooster.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseBooster::AKT_BaseBooster()
{
}


void AKT_BaseBooster::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_BaseBooster::Interactive(AKT_PlayerCharacter* Player)
{
	DisableObject();
	BoostDownTimerDelegate.BindUFunction(this, "BoostDown", Player);
	GetWorldTimerManager().SetTimer(BoostDownTimerHandle, BoostDownTimerDelegate, BoostDownTimer, false);
}

void AKT_BaseBooster::BoostDown(AKT_PlayerCharacter* Player) const 
{
	
}
