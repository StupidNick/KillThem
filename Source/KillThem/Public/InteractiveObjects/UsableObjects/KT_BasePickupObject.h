#pragma once

#include "CoreMinimal.h"

#include "KillThem/Public/InteractiveObjects/KT_BaseInteractiveObject.h"
#include "KT_BasePickupObject.generated.h"



UCLASS()
class KILLTHEM_API AKT_BasePickupObject : public AKT_BaseInteractiveObject
{
	GENERATED_BODY()
	
public:
	AKT_BasePickupObject();

//private C++ functions
private:

	UFUNCTION(Client, Unreliable)
		void TimeLineProgress(float Value);

	
//private C++ variables
private:

	UPROPERTY()
		UTimelineComponent* BoosterTimeLineComponent;
	
//protected C++ functions
protected:

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Unreliable)
		void BoosterActivated(AKT_PlayerCharacter* Player, UTexture2D* Icon, float Time);

	UFUNCTION(NetMulticast, Unreliable)
		void BoosterDeactivated(AKT_PlayerCharacter* Player);

	UFUNCTION()
		virtual void BoostDown(AKT_PlayerCharacter* Player);

	virtual void EnableObject(AKT_PlayerCharacter* Player) override;

	virtual void DisableObject() override;


//protected C++ variables
protected:

	FTimerHandle BoostDownTimerHandle;
	FTimerDelegate BoostDownTimerDelegate;

//public C++ functions
public:

	virtual void InteractiveOnServer(AKT_PlayerCharacter* Player) override;

	FOnTimelineFloat BoosterInterpFunction{};
	
	virtual void RotationTimeLineFloatReturn(float Value) override;
	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USkeletalMeshComponent* SkeletalMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		float BoosterCoolingTime;

	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		bool IsABooster;

	UPROPERTY(EditDefaultsOnly, Category = "Boosts")
		UTexture2D* BoosterIcon;

	UPROPERTY(EditAnywhere, Category = "Curve")
		UCurveFloat* BoosterIndicateCurveFloat;
};
