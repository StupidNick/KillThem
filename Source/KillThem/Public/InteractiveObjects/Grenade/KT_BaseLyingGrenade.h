#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"
#include "KT_BaseLyingGrenade.generated.h"



class AKT_BaseGrenade;



UCLASS()
class KILLTHEM_API AKT_BaseLyingGrenade : public AKT_BaseLyingObject
{
	GENERATED_BODY()
		
public:
	
	AKT_BaseLyingGrenade();


//protected C++ functions
protected:

	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;


//protected C++ variables
protected:

	FTimerHandle BoostDownTimerHandle;
	FTimerDelegate BoostDownTimerDelegate;

	
//public BP variables
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Grenade")
		TSubclassOf<AKT_BaseGrenade> ClassOfGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Grenade")
		bool IsAFirstGrenadeSlot;
};
