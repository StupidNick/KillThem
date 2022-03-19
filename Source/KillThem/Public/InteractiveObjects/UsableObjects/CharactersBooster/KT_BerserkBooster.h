#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BasePickupObject.h"
#include "KT_BerserkBooster.generated.h"



UCLASS()
class KILLTHEM_API AKT_BerserkBooster : public AKT_BasePickupObject
{
	GENERATED_BODY()

				
public:
	
	AKT_BerserkBooster();

//private C++ functions
private:


//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;
	
	virtual void BoostDown(AKT_PlayerCharacter* Player) override;

//protected C++ variables
protected:

	FTimerHandle BoostDownTimerHandle;
	FTimerDelegate BoostDownTimerDelegate;

//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		float BerserkFactor;
};
