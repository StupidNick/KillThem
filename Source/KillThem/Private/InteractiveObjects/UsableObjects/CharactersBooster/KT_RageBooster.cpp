#include "InteractiveObjects/UsableObjects/CharactersBooster/KT_RageBooster.h"

#include "Components/KT_ItemsManagerComponent.h"

AKT_RageBooster::AKT_RageBooster()
{
}


void AKT_RageBooster::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_RageBooster::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	Player->RageBoostOnServer(RageFactor);
}

void AKT_RageBooster::BoostDown(AKT_PlayerCharacter* Player)
{
	Super::BoostDown(Player);
	
	Player->RageBoostOnServer(1 / RageFactor);
}
