#include "InteractiveObjects/Weapons/KT_BaseBoxWithWeapon.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Components/SphereComponent.h"


AKT_BaseBoxWithWeapon::AKT_BaseBoxWithWeapon()
{
	bReplicates = true;
}


void AKT_BaseBoxWithWeapon::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	Player->ItemsManagerComponent->AddWeapon(WeaponClass, AmountOfAmmo);
	OnSphereComponentEndOverlap(InteractSphereCollision, Player, InteractSphereCollision, 0);
}


void AKT_BaseBoxWithWeapon::InteractiveOnClient()
{
	Super::InteractiveOnClient();

	DisableObject();
}
