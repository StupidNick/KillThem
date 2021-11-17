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

//public C++ variable
public:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Scoping)
		bool IsScoping = false;


//public C++ functions
public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(NetMulticast, Reliable)
		void OnRep_Scoping();

	virtual void Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int InAmmoInTheClip = -1) override;
	
	virtual void UseWeapon() override;

	UFUNCTION()
		void ToReload();

	UFUNCTION(Server, Reliable)
		void Scope();

	UFUNCTION()
		void UnScope();

	
	void SetScatterFactor(const float InScatterFactor);

//Protected C++ functions
protected:

	UFUNCTION()
		virtual void ProjectileShoot(const TSubclassOf<AKT_BaseProjectile> InProjectileClass, const int InDamage, const FName InShotSocketName, const float InScatterFactor, const int InSpentAmmo = 1);

	UFUNCTION()
		virtual void LineTraceShot(const TSubclassOf<AKT_BaseProjectile> InProjectileClass, const int InDamage, const FName InShotSocketName, const float InScatterFactor, const int InSpentAmmo = 1);

	UFUNCTION()
		void Reload(const int InAmmo);

//Protected C++ variables
protected:

	UPROPERTY(BlueprintReadOnly)
		bool IsReloading = false;

	UPROPERTY(BlueprintReadOnly)
		float ScatterFactor;

	UPROPERTY(BlueprintReadOnly)
		float AlterFireScatterFactor;

	FTimerHandle ReloadTimerHandle;
	
	FTimerDelegate ReloadTimerDelegate;
	
//public BP variables
public:

	UPROPERTY(EditAnywhere, Category = "Weapon | Components")
		UCameraComponent* CameraComponent;
	

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Info")
		float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Info")
		float MaxDistanceAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		float BaseScatterFactor;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		float ScopeScatterFactor;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		bool ProjectileShooting;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		TSubclassOf<AKT_BaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		TSubclassOf<UDamageType> DamageTypeClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		FName FireSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		bool ProjectileShootingAtAlterFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		TSubclassOf<AKT_BaseProjectile> AlterFireProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		TSubclassOf<UDamageType> AlterDamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		float BaseAlterFireScatterFactor;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		FName AlterFireSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Scoping")
		FName ScopingSocketName;
};
