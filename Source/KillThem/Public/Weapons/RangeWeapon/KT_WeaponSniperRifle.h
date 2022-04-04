#pragma once

#include "CoreMinimal.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"
#include "KT_WeaponSniperRifle.generated.h"



UCLASS()
class KILLTHEM_API AKT_WeaponSniperRifle : public AKT_BaseRangeWeapon
{
	GENERATED_BODY()

//Public C++ functions	
public:
	
	// virtual void UseWeapon() override;


//Public BP variables	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Fire")
		int32 NumbersOfShot;
};
