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

	for (auto i : BlackList)
	{
		if (i == Player) return;
	}
	Player->ItemsManagerComponent->AddWeapon(WeaponClass, AmountOfAmmo);
	OnSphereComponentEndOverlap(InteractSphereCollision, Player, InteractSphereCollision, 0);

	if (RecoverTime > 0)
	{
		EnableTimerDelegate.BindUFunction(this, "ToEnableObject", Player);
		GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
	}

	BlackList.Add(Player);
}


void AKT_BaseBoxWithWeapon::InteractiveOnClient(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnClient(Player);

	for (auto i : BlackList)
	{
		if (i == Player) return;
	}
	DisableObject();

	if (RecoverTime > 0)
	{
		EnableTimerDelegate.BindUFunction(this, "ToEnableObject", Player);
		GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
	}
	
	BlackList.Add(Player);
}


void AKT_BaseBoxWithWeapon::EnableObject(AKT_PlayerCharacter* Player)
{
	Super::EnableObject(Player);

	BlackList.RemoveSingle(Player);
}