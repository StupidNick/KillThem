#include "InteractiveObjects/UsableObjects/CharactersBooster/KT_SpeedBooster.h"



AKT_SpeedBooster::AKT_SpeedBooster()
{
}


void AKT_SpeedBooster::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_SpeedBooster::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);

	Player->ChangeCharacterSpeeds(SpeedFactor);
}


void AKT_SpeedBooster::BoostDown(AKT_PlayerCharacter* Player) const
{
	Super::BoostDown(Player);

	Player->ChangeCharacterSpeeds(1 / SpeedFactor);
}
