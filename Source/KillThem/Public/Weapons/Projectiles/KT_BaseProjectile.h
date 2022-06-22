#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "KT_BaseProjectile.generated.h"



class USphereComponent;
class AKT_BaseRangeWeapon;
class AKT_PlayerCharacter;
class UProjectileMovementComponent;



UCLASS()
class KILLTHEM_API AKT_BaseProjectile : public AActor
{
	GENERATED_BODY()

//Constructor
public:	

	AKT_BaseProjectile();
	
//protected C++ functions
protected:

	FVector ShootDirection;

//protected C++ functions
protected:

	virtual void BeginPlay() override;

//protected C++ functions
protected:

	float Damage;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
		TSubclassOf<UDamageType> GetDamageType(const FHitResult& InHit) const;

	UPROPERTY()
		AKT_PlayerCharacter* PlayerOwner;

	UPROPERTY()
		AKT_BaseRangeWeapon* WeaponInstigator;

	bool IsOnServer = false;
	
//public C++ functions
public:

	UFUNCTION()
		void Initialize(float InDamage, AKT_PlayerCharacter* InPlayerOwner, AKT_BaseRangeWeapon* InWeaponInstigator = nullptr);

	UFUNCTION(NetMulticast, Unreliable)
		void HiddenProjectile();

	UFUNCTION()
		void SetShootDirection(const FVector& Direction);

	UFUNCTION(NetMulticast, UnReliable)
		void SpawnHitEffect(UParticleSystem* InParticleSystem);

	UFUNCTION()
		void SetIsOnServer(bool Value);
	
//public BP variables
public:

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
		USphereComponent* CollisionComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		UParticleSystemComponent* ParticleSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		UParticleSystem* DestroyParticleSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		bool IsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		float ProjectileLifeTime;

	UPROPERTY(EditDefaultsOnly)
		float DamageRadius = 20.f;

	UPROPERTY(EditDefaultsOnly)
		bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage types")
		TSubclassOf<UDamageType> HeadDamageType;
	UPROPERTY(EditDefaultsOnly, Category = "Damage types")
		TSubclassOf<UDamageType> BodyDamageType;
};
