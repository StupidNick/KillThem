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
	
	UFUNCTION(Server, Reliable)
		void OnRep_Scoping();

	virtual void Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int32& InAmmoInTheClip) override;
	
	virtual void UseWeapon() override;

	UFUNCTION(Server, Reliable)
		void ToReload();

	
	void SetScatterFactor(const float InScatterFactor);

//Protected C++ functions
protected:

	UFUNCTION()
		virtual void ProjectileShoot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage, const FName& InShotSocketName, const float& InScatterFactor, const int32& InSpentAmmo = 1);

	UFUNCTION()
		virtual void LineTraceShot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage, const FName& InShotSocketName, const float& InScatterFactor, const int32& InSpentAmmo = 1);

	UFUNCTION()
		bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	UFUNCTION()
		FVector GetMuzzleWorldLocation(const FName& SocketName) const;

	UFUNCTION()
		bool GetTraceData(FVector& StartLocation, FVector& EndLocation, const float InScatterFactor) const;

	UFUNCTION()
		void MakeHit(FHitResult& HitResult, const FVector& StartLocation, const FVector& EndLocation) const;

	UFUNCTION()
		void SpawnProjectile(const FHitResult& HitResult, const FVector& EndLocation, const FName& SocketName, const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage = 0);
	
	UFUNCTION()
		void Reload(const int InAmmo);

//Protected C++ variables
protected:

	UPROPERTY(BlueprintReadOnly)
		float ScatterFactor;

	UPROPERTY(BlueprintReadOnly)
		float AlterFireScatterFactor;

	FTimerHandle ReloadTimerHandle;
	
	FTimerDelegate ReloadTimerDelegate;
	
//public BP variables
public:

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
