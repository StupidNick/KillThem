#pragma once

#include "CoreMinimal.h"
#include "Weapons/KT_BaseWeapon.h"
#include "Weapons/Projectiles/KT_BaseProjectile.h"

#include "KT_BaseRangeWeapon.generated.h"



class AKT_BaseBulletShell;



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

	UFUNCTION(NetMulticast, Unreliable)
		void PlayAnimation(UAnimationAsset* InAnimation);
	
	UFUNCTION(Server, Reliable)
		virtual void ToReload();
	
	void SetScatterFactor(const float InScatterFactor);

//Protected C++ functions
protected:

	UFUNCTION(NetMulticast, Unreliable)
		void SpawnMuzzleFlash(UParticleSystem* MuzzleParticle, const FName& InShotSocketName);

	UFUNCTION(NetMulticast, Unreliable)
		void ToSpawnBulletShell();

	UFUNCTION()
		void SpawnBulletShell() const;

	UFUNCTION(NetMulticast, Unreliable)
		void SpawnFakeProjectile(const FHitResult& HitResult, const FVector& EndLocation, TSubclassOf<AKT_BaseProjectile> InProjectileClass, const FName& SocketName) const;

	UFUNCTION()
		virtual void ProjectileShoot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage, const FName& InShotSocketName, const float& InScatterFactor, UParticleSystem* MuzzleParticle);

	UFUNCTION()
		virtual void LineTraceShot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage, const FName& InShotSocketName, const float& InScatterFactor, UParticleSystem* MuzzleParticle);

	UFUNCTION()
		bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	UFUNCTION()
		FVector GetMuzzleWorldLocation(const FName& SocketName) const;

	UFUNCTION()
		bool GetTraceData(FVector& StartLocation, FVector& EndLocation, const float InScatterFactor) const;

	UFUNCTION()
		void MakeHit(FHitResult& HitResult, const FVector& StartLocation, const FVector& EndLocation) const;

	UFUNCTION(NetMulticast, Reliable)
		void SpawnProjectile(const FHitResult& HitResult, const FVector& EndLocation, const FName& SocketName, TSubclassOf<AKT_BaseProjectile> InProjectileClass, const int32& InDamage = 0);
	
	UFUNCTION()
		virtual void Reload(const int InAmmo);

	UFUNCTION()
		void ClipReloading();

	UFUNCTION()
		void StartBulletReloading();

	UFUNCTION()
		void OneBulletReloading();

	UFUNCTION()
		void EndBulletReloading();

	UFUNCTION()
		void SendBullet();

	UFUNCTION()
		void StopReloading();

	UFUNCTION()
		TSubclassOf<UDamageType> GetDamageType(const FHitResult& InHit) const;

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

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Reload")
		float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Reload")
		float SendBulletTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Reload")
		float StartReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Reload")
		float EndReloadTime;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon | Reload")
		bool IsOneBulletReloading = false;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon | Reload")
		bool IsSendingBullet = false;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon | Reload")
		bool IsEndingReload = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Reload")
		bool NeedSendingOneBullet = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Reload")
		bool ClipReload = true;

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

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Damage")
		TSubclassOf<UDamageType> HeadDamageType;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Damage")
		TSubclassOf<UDamageType> BodyDamageType;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		UParticleSystem* AlterFireParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Scoping")
		FName ScopingSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animations")
		UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animations")
		UAnimationAsset* ShootAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animations")
		UAnimationAsset* StartReloadingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animations")
		UAnimationAsset* EndReloadingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Animations")
		UAnimationAsset* SendBulletAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
		UParticleSystem* FireParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
		float SpawnBulletShellDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
		TSubclassOf<AKT_BaseBulletShell> BulletShellClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
		FName BulletShellSocketName;
};
