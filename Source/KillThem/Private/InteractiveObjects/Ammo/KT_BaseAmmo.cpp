#include "InteractiveObjects/Ammo/KT_BaseAmmo.h"

#include "Components/KT_ItemsManagerComponent.h"


AKT_BaseAmmo::AKT_BaseAmmo()
{
}


void AKT_BaseAmmo::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);

	Player->ItemsManagerComponent->AddAmmo(ClassOfAmmo, CountOfAmmo);
}
