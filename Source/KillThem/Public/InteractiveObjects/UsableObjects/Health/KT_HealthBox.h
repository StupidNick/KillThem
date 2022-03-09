#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"
#include "KT_HealthBox.generated.h"



UCLASS()
class KILLTHEM_API AKT_HealthBox : public AKT_BaseLyingObject
{
	GENERATED_BODY()

		
public:
	AKT_HealthBox();

//private C++ functions
private:


//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;

//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float RecoveryHealth;
};
