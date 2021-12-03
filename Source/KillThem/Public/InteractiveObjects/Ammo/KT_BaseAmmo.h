#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"
#include "KT_BaseAmmo.generated.h"



UCLASS()
class KILLTHEM_API AKT_BaseAmmo : public AKT_BaseLyingObject
{
	GENERATED_BODY()
		
public:
	
	AKT_BaseAmmo();

//public C++ functions
public:

	void Initialize(int InAmountAmmo);
	
//protected C++ functions
protected:

	virtual void Interactive(AKT_PlayerCharacter* Player) override;


//protected C++ variables
protected:

	FTimerHandle BoostDownTimerHandle;
	FTimerDelegate BoostDownTimerDelegate;

	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		TSubclassOf<AKT_BaseWeapon> ClassOfAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		int CountOfAmmo;
};
