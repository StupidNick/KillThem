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
	
	AKT_BaseRangeWeapon();

//public C++ functions
public:


	virtual void Initialize_Implementation(AKT_PlayerCharacter* InCharacter) override;
	
	virtual void UseWeapon() override;

	UFUNCTION()
		void ToReload();

//Protected C++ functions
protected:

	UFUNCTION()
		virtual void ProjectileShoot();

	UFUNCTION()
		virtual void LineTraceProjectile();

	UFUNCTION()
		void Reload(const int InAmmo);

//Protected C++ variables
protected:

	UPROPERTY(BlueprintReadOnly)
		int AmmoInTheClip;

	UPROPERTY(BlueprintReadOnly)
		bool IsReloading = false;

	FTimerHandle ReloadTimerHandle;
	
	FTimerDelegate ReloadTimerDelegate;
	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		int ClipSize;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float ReloadTime;

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
