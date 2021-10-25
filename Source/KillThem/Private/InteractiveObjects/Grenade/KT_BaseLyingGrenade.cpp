#include "InteractiveObjects/Grenade/KT_BaseLyingGrenade.h"

#include "Components/KT_ItemsManagerComponent.h"


AKT_BaseLyingGrenade::AKT_BaseLyingGrenade()
{
}


void AKT_BaseLyingGrenade::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);

	if (HasAuthority())
	{
		Player->AddGrenade(ClassOfGrenade, IsAFirstGrenadeSlot);
	}
}
