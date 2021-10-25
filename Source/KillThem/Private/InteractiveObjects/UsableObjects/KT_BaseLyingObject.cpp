#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseLyingObject::AKT_BaseLyingObject()
{
}


void AKT_BaseLyingObject::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_BaseLyingObject::Interactive(AKT_PlayerCharacter* Player)
{
	DisableObject();
	if (IsABooster)
	{
		BoostDownTimerDelegate.BindUFunction(this, "BoostDown", Player);
		GetWorldTimerManager().SetTimer(BoostDownTimerHandle, BoostDownTimerDelegate, BoostDownTimer, false);
	}
}

void AKT_BaseLyingObject::BoostDown(AKT_PlayerCharacter* Player) const 
{
	
}
