#include "InteractiveObjects/Weapons/KT_BaseBoxWithWeapon.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Components/SphereComponent.h"



AKT_BaseBoxWithWeapon::AKT_BaseBoxWithWeapon()
{
}


void AKT_BaseBoxWithWeapon::Interactive(AKT_PlayerCharacter* Player)
{
	if (!HasAuthority())
	{
		DisableObject();
		if (RecoverTime > 0)
		{
			EnableTimerDelegate.BindUFunction(this, "EnableObject");
			GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
		}
	}
	
	if (HasAuthority())
	{
		Player->ItemsManagerComponent->AddWeapon(WeaponClass, AmountOfAmmo);
		OnSphereComponentEndOverlap(InteractSphereCollision, Player, InteractSphereCollision, 0);
	}
}
