#pragma once

#include "CoreMinimal.h"
#include "Weapons/KT_BaseWeapon.h"
#include "Weapons/Projectiles/KT_BaseProjectile.h"

#include "KT_BaseRangeWeapon.generated.h"



UCLASS()
class KILLTHEM_API AKT_BaseRangeWeapon : public AKT_BaseWeapon
{
	GENERATED_BODY()

//Constructor
public:

	virtual void UseWeapon() override;

	UFUNCTION()
		virtual void ProjectileShoot();

	UFUNCTION()
		virtual void LineTraceProjectile();

//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float MaxDistanceAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName FireSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool ProjectileShooting;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AKT_BaseProjectile> ProjectileClass;
};
