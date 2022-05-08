#pragma once

#include "CoreMinimal.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"
#include "KT_WeaponShotgun.generated.h"



UCLASS()
class KILLTHEM_API AKT_WeaponShotgun : public AKT_BaseRangeWeapon
{
	GENERATED_BODY()

	AKT_WeaponShotgun();

//Private C++ functions	
private:

//Protected C++ functions	
protected:

//Public C++ functions	
public:
	
	virtual void UseWeapon() override;


//Public BP variables	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		int32 NumbersOfShot;
};
