#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BaseBooster.h"
#include "KT_HealthBox.generated.h"



UCLASS()
class KILLTHEM_API AKT_HealthBox : public AKT_BaseBooster
{
	GENERATED_BODY()

		
public:
	AKT_HealthBox();

//private C++ functions
private:


//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void Interactive(AKT_PlayerCharacter* Player) override;

//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float RecoveryHealth;
};
