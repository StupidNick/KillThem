#include "InteractiveObjects/Weapons/KT_BaseBoxWithWeapon.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/BoxComponent.h"


AKT_BaseBoxWithWeapon::AKT_BaseBoxWithWeapon()
{
}


void AKT_BaseBoxWithWeapon::Interactive(AActor* OtherActor)
{
	if (!HasAuthority())
	{
		BoxCollision->SetGenerateOverlapEvents(false);
		DisableObject();
		if (RecoverTime > 0)
		{
			GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
		}
	}
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		LCharacter->AddWeapon(WeaponClass, AmountOfAmmo);
	}
}
