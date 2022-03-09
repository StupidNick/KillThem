#include "InteractiveObjects/UsableObjects/CharactersBooster/KT_SpeedBooster.h"



AKT_SpeedBooster::AKT_SpeedBooster()
{
}


void AKT_SpeedBooster::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_SpeedBooster::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	Player->SpeedBoostOnServer(SpeedFactor);
}


void AKT_SpeedBooster::BoostDown(AKT_PlayerCharacter* Player)
{
	Super::BoostDown(Player);

	Player->SpeedBoostOnServer(1 / SpeedFactor);
}
