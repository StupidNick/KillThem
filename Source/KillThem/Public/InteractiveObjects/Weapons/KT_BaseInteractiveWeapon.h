#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/KT_BaseInteractiveObject.h"

#include "KT_BaseInteractiveWeapon.generated.h"



class AKT_BaseWeapon;



UCLASS()
class KILLTHEM_API AKT_BaseInteractiveWeapon : public AKT_BaseInteractiveObject
{
	GENERATED_BODY()

//Constructors
public:	

	AKT_BaseInteractiveWeapon();

//private C++ functions
private:

	UFUNCTION()
		void Destruction();

//private C++ functions
private:

	UPROPERTY()
		int AmmoInTheClip;
	
//protected C++ functions
protected:
	
	virtual void Interactive(AKT_PlayerCharacter* Player) override;

//public C++ functions
public:

	UFUNCTION()
		void Initialize(int InAmountOfAmmo);

//public BP variables
public:

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		TSubclassOf<AKT_BaseWeapon> WeaponClass = nullptr;
};
