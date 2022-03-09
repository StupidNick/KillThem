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
	
//private C++ functions
private:


//private C++ variables
private:
	

//Protected C++ functions
protected:

	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;
	
	virtual void InteractiveOnClient() override;
	

//Public C++ variables
public:
	
//public BP variables
public:
	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		TSubclassOf<AKT_BaseWeapon> WeaponClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		int AmountOfAmmo = 0;
	
};
