#include "InteractiveObjects/Grenade/KT_BaseLyingGrenade.h"

#include "Components/KT_ItemsManagerComponent.h"


AKT_BaseLyingGrenade::AKT_BaseLyingGrenade()
{
}


void AKT_BaseLyingGrenade::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	if (HasAuthority())
	{
		// Player->AddGrenade(ClassOfGrenade, IsAFirstGrenadeSlot);
	}
}
