#include "Weapons/RangeWeapon/KT_WeaponShotgun.h"

#include "Components/KT_ItemsManagerComponent.h"



AKT_WeaponShotgun::AKT_WeaponShotgun()
{
	ReloadTimerDelegate.BindUFunction(this, "Reload", 1);
}


void AKT_WeaponShotgun::UseWeapon()
{
	if (AmmoInTheClip <= 0 || IsReloading || !GetWeaponCanShoot()) return;
	StopReload();
	
	if (UseAlterFire)
	{
		if (AmmoInTheClip < CostAlterShotInAmmo) return;

		ProjectileShoot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, AlterFireScatterFactor, AlterFireParticle);
		AmmoInTheClip -= CostAlterShotInAmmo;
	}
	else
	{
		for (int32 LCounter = 0; LCounter < NumbersOfShot; LCounter++)
		{
			LineTraceShot(ProjectileClass, Damage, FireSocketName, ScatterFactor, FireParticle);
		}
		AmmoInTheClip--;
	}
	ActivateTimerBetweenShots();
}


void AKT_WeaponShotgun::Reload(const int InAmmo)
{
	Super::Reload(InAmmo);

	int32 LCountOfAmmo;
	if (!Character->ItemsManagerComponent->CountAmmo(GetClass(), LCountOfAmmo) || AmmoInTheClip >= ClipSize) return;

	ToReload();
}


void AKT_WeaponShotgun::ToReload()
{
	if (IsChangingFireMode || !IsValid(Character)) return;

	int32 LCountOfAmmo;
	if (!Character->ItemsManagerComponent->CountAmmo(GetClass(), LCountOfAmmo) || AmmoInTheClip >= ClipSize) return;

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadTimerDelegate, ReloadTime / Character->BerserkBooster,
									false);
}


void AKT_WeaponShotgun::StopReload()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}


