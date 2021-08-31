#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "KillThem/Public/InteractiveObjects/KT_BaseInteractiveObject.h"
#include "KT_HealthBox.generated.h"


UCLASS()
class KILLTHEM_API AKT_HealthBox : public AKT_BaseInteractiveObject
{
	GENERATED_BODY()
	
public:
	AKT_HealthBox();

//private C++ functions
private:

	UTimelineComponent* RotationTimeLine;

	FOnTimelineFloat RotationInterpFunction{};

	UFUNCTION()
		void RotationTimeLineFloatReturn(float Value);
	
	void HealthRecovery(AActor* OtherActor);

//protected C++ functions
protected:

	virtual void BeginPlay() override;

	virtual void Interactive(AActor* OtherActor) override;

//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Rotation")
		float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		UCurveFloat* CurveFloatForRotation;
};
