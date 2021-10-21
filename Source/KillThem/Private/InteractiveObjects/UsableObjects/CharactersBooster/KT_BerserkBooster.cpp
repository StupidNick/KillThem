#include "InteractiveObjects/UsableObjects/CharactersBooster/KT_BerserkBooster.h"

AKT_BerserkBooster::AKT_BerserkBooster()
{
}

void AKT_BerserkBooster::BeginPlay()
{
	Super::BeginPlay();
}

void AKT_BerserkBooster::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);

	Player->BerserkBooster = BerserkFactor;
}

void AKT_BerserkBooster::BoostDown(AKT_PlayerCharacter* Player) const
{
	Super::BoostDown(Player);
	
	Player->BerserkBooster = 1;
}
