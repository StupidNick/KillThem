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

	UFUNCTION(NetMulticast, Reliable)
		void Destruction();

//private C++ variables
private:

	UPROPERTY(Replicated)
		int16 AmmoInTheClip;
	
//protected C++ functions
protected:
	
	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;

//public C++ functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
		void Initialize(const int16& InAmountOfAmmo);

//public BP variables
public:

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		TSubclassOf<AKT_BaseWeapon> WeaponClass = nullptr;
};
