#pragma once

#include "CoreMinimal.h"
#include "Character/Controllers/KT_PlayerController.h"


#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjects/Weapons/KT_BaseInteractiveWeapon.h"

#include "KT_BaseWeapon.generated.h"


class UImage;
class AKT_PlayerCharacter;



UCLASS()
class KILLTHEM_API AKT_BaseWeapon : public AActor
{
	GENERATED_BODY()

//Constructors
public:	

	AKT_BaseWeapon();

//private C++ functions
private:

//private C++ variables
private:

	
	
//private C++ functions
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void AutoFireReload();

	UFUNCTION()
		void ActivateTimerBetweenShots();

	UFUNCTION(Server, Reliable)
		void EndChangeFireMode();

	UFUNCTION()
		bool GetWeaponCanShoot() const;

//protected C++ variables
protected:

	bool CanShoot = true;

	bool IsChangingFireMode = false;

	UPROPERTY(BlueprintReadOnly, Transient, ReplicatedUsing = OnRep_AmmoInTheClip, Category = "Stats")
		int AmmoInTheClip;

	UPROPERTY(Replicated)
		bool UseAlterFire;
	
	FTimerHandle AutoFireTimerHandle;
	FTimerHandle ChangeFireModeTimerHandle;
	
	FTimerDelegate AutoFireTimerDelegate;
	FTimerDelegate ChangeFireModeTimerDelegate;

//public C++ functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_AmmoInTheClip();

	UFUNCTION()
		virtual void UseWeapon();

	UFUNCTION(Server, Reliable)
		void Initialize(AKT_PlayerCharacter* InCharacter, const int32& InAmmoInTheClip = -1);

	UFUNCTION(Server, Reliable)
		void StartChangeFireMode();

	UFUNCTION(Server, Reliable)
		void BreakAllAction();

	UFUNCTION()
	FORCEINLINE int GetAmmoInTheClip() const
	{
		return AmmoInTheClip;
	}
	

//public C++ variables
public:

	UPROPERTY(BlueprintReadOnly, Replicated)
		bool IsReloading = false;

	
//public BP variables
public:

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Character")
		AKT_PlayerCharacter* Character;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Character")
		AKT_PlayerController* Controller;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Components")
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Info")
		int ClipSize;

	UPROPERTY(EditAnywhere, Category = "Weapon | Info")
		UTexture2D* WeaponIcon;
	
	UPROPERTY(EditAnywhere, Category = "Weapon | Info")
		UTexture2D* AimIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		float DelayBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		bool AutoFire;

/////////////////////////AltFire////////////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		float ChangeFireModeTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		float AlterDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		bool AutoFireAtAlterFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		float DelayBetweenAlterShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		int CostAlterShotInAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Scoping")
		bool CanScope;

	UPROPERTY(EditAnywhere, Category = "Weapon | Interactive")
		TSubclassOf<AKT_BaseInteractiveWeapon> InteractiveWeaponClass = nullptr;
};
