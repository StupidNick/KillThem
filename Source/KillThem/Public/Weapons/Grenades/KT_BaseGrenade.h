#pragma once

#include "CoreMinimal.h"
#include "Weapons/KT_BaseWeapon.h"
#include "KT_BaseGrenade.generated.h"



class AKT_BaseProjectile;



UCLASS()
class KILLTHEM_API AKT_BaseGrenade : public AKT_BaseWeapon
{
	GENERATED_BODY()
		
public:
	
	AKT_BaseGrenade();

//private C++ functions
private:

	
//private C++ functions
protected:

	// virtual void UseWeapon() override;

	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character | Grenade")
		FName SpawnSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Grenade")
		const TSubclassOf<AKT_BaseProjectile> ProjectileClass;
};
