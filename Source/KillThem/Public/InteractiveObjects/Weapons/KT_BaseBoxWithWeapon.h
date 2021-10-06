#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/KT_BaseInteractiveObject.h"
#include "KT_BaseBoxWithWeapon.generated.h"



class AKT_BaseWeapon;



UCLASS()
class KILLTHEM_API AKT_BaseBoxWithWeapon : public AKT_BaseInteractiveObject
{
	GENERATED_BODY()

//Constructors
public:
	
	AKT_BaseBoxWithWeapon();
	
//Protected C++ functions
protected:

	UFUNCTION()
		virtual void Interactive(AActor* OtherActor) override;

//Protected C++ variables
protected:
	
	
	
//public BP variables
public:
	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		TSubclassOf<AKT_BaseWeapon> WeaponClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		int AmountOfAmmo = 0;
};
