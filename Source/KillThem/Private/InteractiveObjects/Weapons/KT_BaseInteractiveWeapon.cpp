#include "InteractiveObjects/Weapons/KT_BaseInteractiveWeapon.h"

#include "Weapons/KT_BaseWeapon.h"
#include "Character/KT_PlayerCharacter.h"


AKT_BaseInteractiveWeapon::AKT_BaseInteractiveWeapon()
{
	
}


void AKT_BaseInteractiveWeapon::Interactive(AActor* OtherActor)
{
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		if (HasAuthority())
		{
			LCharacter->AddWeapon(WeaponClass, 0);
			Destroy();
		}
	}
	
}


void AKT_BaseInteractiveWeapon::Initialize(int InAmountOfAmmo)
{
}