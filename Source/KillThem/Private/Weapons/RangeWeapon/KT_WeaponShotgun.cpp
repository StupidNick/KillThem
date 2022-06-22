#include "Weapons/RangeWeapon/KT_WeaponShotgun.h"

#include "Components/KT_ItemsManagerComponent.h"



AKT_WeaponShotgun::AKT_WeaponShotgun()
{
	
}


void AKT_WeaponShotgun::UseWeapon()
{
	if (AmmoInTheClip <= 0 || !GetWeaponCanShoot()) return;
	if (IsReloading)
	{
		EndBulletReloading();
		return;
	}

	IsShoot = true;
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
	PlayAnimation(ShootAnimation);
	ToSpawnBulletShell();
}