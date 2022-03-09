#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"
#include "KT_SpeedBooster.generated.h"



UCLASS()
class KILLTHEM_API AKT_SpeedBooster : public AKT_BaseLyingObject
{
	GENERATED_BODY()
				
public:
	
	AKT_SpeedBooster();

//private C++ functions
private:


//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;
	
	virtual void BoostDown(AKT_PlayerCharacter* Player) override;

//protected C++ variables
protected:


//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		float SpeedFactor;
};
