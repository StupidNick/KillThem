#include "InteractiveObjects/Ammo/KT_BaseAmmo.h"

#include "Components/KT_ItemsManagerComponent.h"


AKT_BaseAmmo::AKT_BaseAmmo()
{
}


void AKT_BaseAmmo::Initialize(int InAmountAmmo)
{
	if (InAmountAmmo > 0)
	{
		CountOfAmmo = InAmountAmmo;
	}
	else
	{
		Destroy();
	}
}


void AKT_BaseAmmo::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	if (HasAuthority())
	{
		Player->ItemsManagerComponent->AddAmmoOnServer(ClassOfAmmo, CountOfAmmo);
	}
}
