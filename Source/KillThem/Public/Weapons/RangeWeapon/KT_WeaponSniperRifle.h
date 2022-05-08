#pragma once

#include "CoreMinimal.h"
#include "UI/WeaponsWD/KT_SniperScopeWD.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"
#include "KT_WeaponSniperRifle.generated.h"



UCLASS()
class KILLTHEM_API AKT_WeaponSniperRifle : public AKT_BaseRangeWeapon
{
	GENERATED_BODY()
//Public C++ functions	
public:

	float ActualFoV;
	
//Public C++ functions	
public:
	
	// virtual void UseWeapon() override;

	virtual void Scope() override;
	virtual void UnScope() override;

//Public BP variables	
public:

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "70"), Category = "Weapon | Fire")
		float ScopeFOV = 20.0f;
};
