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
	
	void StopReload();

//Protected C++ functions	
protected:	

	virtual void Reload(const int InAmmo);

//Public C++ functions	
public:
	
	virtual void UseWeapon() override;

	virtual void ToReload();


//Public BP variables	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		int32 NumbersOfShot;
};
