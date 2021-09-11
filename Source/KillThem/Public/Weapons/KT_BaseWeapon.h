#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Actor.h"
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
	UFUNCTION(Server, Reliable)
		void UseWeaponOnServer();

//private C++ variables
protected:

	bool CanShoot = true;

//public C++ variables
public:

	UFUNCTION()
		void ToUseWeapon();

	UFUNCTION()
		void Initialize(AKT_PlayerCharacter* InCharacter);

	UFUNCTION()
		void ToAttachToComponent(USkeletalMeshComponent*& InComponent, const FName InSocketName);

	UFUNCTION()
		void ToDetachFromActor();
	
//public BP variables
public:

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		AKT_PlayerCharacter* Character;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float DelayBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool AutoFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Damage;
};
