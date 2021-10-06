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

	
//protected C++ functions
protected:

	UFUNCTION()
		virtual void Interactive(AActor* OtherActor) override;

//public C++ functions
public:

	UFUNCTION()
		void Initialize(int InAmountOfAmmo);

//public BP variables
public:

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		TSubclassOf<AKT_BaseWeapon> WeaponClass = nullptr;
};
