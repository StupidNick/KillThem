#pragma once

#include "CoreMinimal.h"
#include "KT_CoreTypes.h"
#include "InteractiveObjects/UsableObjects/KT_BasePickupObject.h"
#include "KT_BaseAmmo.generated.h"



UCLASS()
class KILLTHEM_API AKT_BaseAmmo : public AKT_BaseInteractiveObject
{
	GENERATED_BODY()
		
public:
	
	AKT_BaseAmmo();

//public C++ functions
public:

	UPROPERTY()
		bool IsDroppedAmmo = false;

//public C++ functions
public:

	UFUNCTION(NetMulticast, Reliable)
		void Destruction();

//public C++ functions
public:

	void Initialize(int16 InAmountAmmo, bool IsDropped = false);
	
//protected C++ functions
protected:

	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;


//protected C++ variables
protected:

	FTimerHandle BoostDownTimerHandle;
	FTimerDelegate BoostDownTimerDelegate;

	FGameData GameData;
	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		TSubclassOf<AKT_BaseWeapon> ClassOfAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		int16 CountOfAmmo;
};
