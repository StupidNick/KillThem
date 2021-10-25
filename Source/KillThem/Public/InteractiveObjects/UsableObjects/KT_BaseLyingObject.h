#pragma once

#include "CoreMinimal.h"

#include "KillThem/Public/InteractiveObjects/KT_BaseInteractiveObject.h"
#include "KT_BaseLyingObject.generated.h"


UCLASS()
class KILLTHEM_API AKT_BaseLyingObject : public AKT_BaseInteractiveObject
{
	GENERATED_BODY()
	
public:
	AKT_BaseLyingObject();

//private C++ functions
private:


//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void Interactive(AKT_PlayerCharacter* Player) override;

	UFUNCTION()
		virtual void BoostDown(AKT_PlayerCharacter* Player) const;


//protected C++ variables
protected:

	FTimerHandle BoostDownTimerHandle;
	FTimerDelegate BoostDownTimerDelegate;

	
//public BP variables
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		float BoostDownTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		bool IsABooster;
};