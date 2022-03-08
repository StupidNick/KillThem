#pragma once

#include "CoreMinimal.h"


#include "Character/KT_PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "KT_BaseInteractiveObject.generated.h"



class UBoxComponent;
class USphereComponent;



UCLASS()
class KILLTHEM_API AKT_BaseInteractiveObject : public AActor
{
	GENERATED_BODY()

//Constructors
public:	

	AKT_BaseInteractiveObject();
	
	
//private C++ functions
private:
	
	UTimelineComponent* RotationTimeLine;

//protected C++ functions
protected:

	virtual void BeginPlay() override;

	UFUNCTION(Server, Unreliable)
		void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Unreliable)
		void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Unreliable)
		void OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(NetMulticast, Unreliable)
		virtual void Interactive(AKT_PlayerCharacter* Player);

	UFUNCTION(Server, Reliable)
		virtual void ToEnableObject();

	UFUNCTION(NetMulticast, Reliable)
		virtual void EnableObject();

	UFUNCTION(NetMulticast, Reliable)
		virtual void DisableObject();


	FTimerHandle EnableTimerHandle;
	FTimerDelegate EnableTimerDelegate;

//protected C++ variables
protected:

	UPROPERTY()
		AKT_PlayerCharacter* PlayerCharacter = nullptr;

	bool CanTake = true;

	FTimerHandle RotationTimerHandle;
	FTimerDelegate RotationTimerDelegate;
	
//public C++ functions
public:

	FOnTimelineFloat RotationInterpFunction{};

	UFUNCTION(Server, Unreliable)
		void ToInteractive(AKT_PlayerCharacter* Player);

	UFUNCTION()
		virtual void RotationTimeLineFloatReturn(float Value);
	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USphereComponent* InteractSphereCollision = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* StandStaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "WorkInfo")
		float RecoverTime;

	UPROPERTY(EditDefaultsOnly, Category = "WorkInfo")
		bool InteractOnPressButton;

	UPROPERTY(EditDefaultsOnly, Category = "WorkInfo")
		bool RotateObject = false;

	UPROPERTY(EditDefaultsOnly, Category = "WorkInfo")
		float SpeedRotation;

	UPROPERTY(EditAnywhere, Category = "WorkInfo")
		UCurveFloat* RotationCurve;
};
