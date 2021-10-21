#include "InteractiveObjects/UsableObjects/CharactersBooster/KT_RageBooster.h"

#include "Components/KT_ItemsManagerComponent.h"

AKT_RageBooster::AKT_RageBooster()
{
}


void AKT_RageBooster::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_RageBooster::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);

	Player->DamageBooster = RageFactor;
}

void AKT_RageBooster::BoostDown(AKT_PlayerCharacter* Player) const
{
	Player->DamageBooster = 1;
}
