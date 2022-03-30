#include "InteractiveObjects/UsableObjects/CharactersBooster/KT_BerserkBooster.h"

AKT_BerserkBooster::AKT_BerserkBooster()
{
}

void AKT_BerserkBooster::BeginPlay()
{
	Super::BeginPlay();
}

void AKT_BerserkBooster::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	Player->SetBerserkBoost(BerserkFactor);
}

void AKT_BerserkBooster::BoostDown(AKT_PlayerCharacter* Player)
{
	Super::BoostDown(Player);
	
	Player->SetBerserkBoost(1);
}
