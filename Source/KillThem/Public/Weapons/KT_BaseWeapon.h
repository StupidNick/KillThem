#pragma once

#include "CoreMinimal.h"


#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjects/Weapons/KT_BaseInteractiveWeapon.h"

#include "KT_BaseWeapon.generated.h"


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
		virtual void UseWeapon();

	UFUNCTION()
		void AutoFireReload();

//private C++ variables
protected:

	UPROPERTY(BlueprintReadOnly)
		int AmmoInTheClip;

	UPROPERTY(BlueprintReadOnly)
		bool UseAlterFire;
	
	FTimerHandle AutoFireTimerHandle;
	FTimerHandle AlterFireHandle;
	
	FTimerDelegate AutoFireTimerDelegate;
	FTimerDelegate AlterFireTimerDelegate;

//public C++ variables
public:

	UFUNCTION()
		void ToUseWeapon(const bool IsAlterFire);

	UFUNCTION(BlueprintNativeEvent)
		void Initialize(AKT_PlayerCharacter* InCharacter);
		virtual void Initialize_Implementation(AKT_PlayerCharacter* InCharacter);

	UFUNCTION(NetMulticast, Reliable)
		void ToAttachToComponent(USkeletalMeshComponent* InComponent, const FName InSocketName);

	UFUNCTION(NetMulticast, Reliable)
		void ToDetachFromActor();

	UFUNCTION(NetMulticast, Reliable)
		void Detach();
	

//public C++ variables
public:

	bool CanShoot = true;
	
//public BP variables
public:

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		AKT_PlayerCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Components")
		UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Components")
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		float DelayBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		float TimeBeforeAlterFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		bool AutoFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		bool CanScope;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | AlterFire")
		float AlterDamage;

	UPROPERTY(EditAnywhere, Category = "Weapon | Interactive")
		TSubclassOf<AKT_BaseInteractiveWeapon> InteractiveWeaponClass = nullptr;
};
