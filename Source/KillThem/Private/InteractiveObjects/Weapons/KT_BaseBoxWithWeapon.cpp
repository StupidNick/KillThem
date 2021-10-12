#include "InteractiveObjects/Weapons/KT_BaseBoxWithWeapon.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/SphereComponent.h"


AKT_BaseBoxWithWeapon::AKT_BaseBoxWithWeapon()
{
}


void AKT_BaseBoxWithWeapon::Interactive(AActor* OtherActor)
{
	if (!HasAuthority())
	{
		InteractSphereCollision->SetGenerateOverlapEvents(false);
		DisableObject();
		if (RecoverTime > 0)
		{
			EnableTimerDelegate.BindUFunction(this, "EnableObject");
			GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
		}
	}
	
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		if (HasAuthority())
		{
			LCharacter->AddWeapon(WeaponClass, AmountOfAmmo);
			OnSphereComponentEndOverlap(InteractSphereCollision, OtherActor, InteractSphereCollision, 0);
		}
	}
}
