#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"
#include "KT_ShieldBox.generated.h"




UCLASS()
class KILLTHEM_API AKT_ShieldBox : public AKT_BaseLyingObject
{
	GENERATED_BODY()

public:
	AKT_ShieldBox();

//private C++ functions
private:


//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void Interactive(AKT_PlayerCharacter* Player) override;


//protected C++ variables
protected:

	
//public BP variables
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Shield")
		float ShieldValue;
};