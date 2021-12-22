#include "InteractiveObjects/Weapons/KT_BaseInteractiveWeapon.h"

#include "Weapons/KT_BaseWeapon.h"
#include "Character/KT_PlayerCharacter.h"


AKT_BaseInteractiveWeapon::AKT_BaseInteractiveWeapon()
{
	bReplicates = true;
}


void AKT_BaseInteractiveWeapon::Interactive(AKT_PlayerCharacter* Player)
{
	if (HasAuthority())
	{
		Player->AddWeapon(WeaponClass, 0, AmmoInTheClip);
		Destroy();
	}
}


void AKT_BaseInteractiveWeapon::Initialize(int InAmountOfAmmo)
{
	if (HasAuthority())
	{
		AmmoInTheClip = InAmountOfAmmo;
	}
}