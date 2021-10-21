#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BaseBooster.h"
#include "KT_RageBooster.generated.h"



UCLASS()
class KILLTHEM_API AKT_RageBooster : public AKT_BaseBooster
{
	GENERATED_BODY()

			
public:
	
	AKT_RageBooster();

//private C++ functions
private:


//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void Interactive(AKT_PlayerCharacter* Player) override;
	
	virtual void BoostDown(AKT_PlayerCharacter* Player) const override;

//protected C++ variables
protected:


//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		float RageFactor;
};
