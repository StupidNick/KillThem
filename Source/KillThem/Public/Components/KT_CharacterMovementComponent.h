#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KT_CharacterMovementComponent.generated.h"


class UTimelineComponent;
class AKT_PlayerCharacter;



UCLASS()
class KILLTHEM_API UKT_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UKT_CharacterMovementComponent();
	
//Private C++ functions
private:

	UFUNCTION()
		void Initialize();

	UFUNCTION(NetMulticast, Reliable)
		void CrouchingTimeLineFloatReturn(float Value);

/////////////////////////////////////Slide/////////////////////////////////////////

	UFUNCTION(Server, Reliable)
		void Sliding();
	UFUNCTION()
		bool GetCanSliding() const;
	
	UFUNCTION(NetMulticast, Reliable)
		void SlidingTimeLineFloatReturn(float Value);

	UFUNCTION(NetMulticast, Reliable)
		void WallRunningTimeLineFloatReturn(float Value);

	UFUNCTION()
		void TiltCameraOnWallRunningTimeLineFloatReturn(float Value);

/////////////////////////////////////Dash/////////////////////////////////////////
	
	UFUNCTION(Server, Reliable)
		void DashRecoveringTimeLineFloatReturn(float Value);

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_DashRecoveryCounterUpdate();
	UFUNCTION(NetMulticast, Reliable)
		void DashUsed();

	UFUNCTION(Server, Reliable)
		void DashRecoveryFinished();

	UFUNCTION()
		bool GetCanDash() const;

//Private C++ variables
private:

/////////////////////////////////////Timelines/////////////////////////////////////////

	UPROPERTY()
		UTimelineComponent* SlidingTimeLine;

	UPROPERTY()
		UTimelineComponent* CrouchingTimeLine;

	UPROPERTY()
		UTimelineComponent* WallRunningTimeLine;

	UPROPERTY()
		UTimelineComponent* TiltCameraOnWallRunningTimeLine;

	UPROPERTY()
		UTimelineComponent* DashRecoverTimeLine;

/////////////////////////////////////Dash/////////////////////////////////////////
	
	UPROPERTY(Replicated)
		int32 DashCounter = 0;

	UPROPERTY(ReplicatedUsing = OnRep_DashRecoveryCounterUpdate)
		float DashRecoveryCounter = 0.0f;

	float DashRecoveryValuePerTick = 0.0f;

/////////////////////////////////////WallRun/////////////////////////////////////////
	
	UPROPERTY()
		bool OnWall;

	UPROPERTY()
		bool CameraTiltToRight;

	UPROPERTY()
		bool CameraIsTilt = false;

	UPROPERTY()
		FVector PlayerDirectionForWallRunning;

/////////////////////////////////////Crouch/////////////////////////////////////////
	
	UPROPERTY()
		FVector CrouchingStartLocation;
	UPROPERTY()
		FVector CrouchingEndLocation;

//Protected C++ functions
protected:

	virtual void BeginPlay() override;

//public C++ functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/////////////////////////////////////Timelines/////////////////////////////////////////
	
	FOnTimelineFloat SlidingInterpFunction{};

	FOnTimelineFloat CrouchingInterpFunction{};

	FOnTimelineFloat WallRunningInterpFunction{};

	FOnTimelineFloat TiltCameraOnWallRunningInterpFunction{};

	FOnTimelineFloat DashRecoveringInterpFunction{};

/////////////////////////////////////Sprint/////////////////////////////////////////

	UFUNCTION(Server, Reliable)
		void ChangeSprint();

	UFUNCTION(Server, Reliable)
		void Sprint();
	UFUNCTION(Server, Reliable)
		void UnSprint();

	UFUNCTION(BlueprintCallable)
		bool IsSprinting() const;

/////////////////////////////////////Moving/////////////////////////////////////////

	virtual float GetMaxSpeed() const override;
	
	UFUNCTION(Client, Reliable)
		void MoveForward(float Value);
	UFUNCTION(Client, Reliable)
		void MoveRight(float Value);
	
	UFUNCTION(Server, Reliable)
		void SetMoveForwardOnServer(const float& Value);
	UFUNCTION(Server, Reliable)
		void SetMoveRightOnServer(const float& Value);

	UFUNCTION()
		bool GetIsMovingForward() const;

/////////////////////////////////////Crouch/////////////////////////////////////////

	UFUNCTION(Server, Reliable)
		void ChangeCrouch();
	
	UFUNCTION(Server, Reliable)
		void Crouching();
	UFUNCTION(Server, Reliable)
		void UnCrouching();

/////////////////////////////////////Jump/////////////////////////////////////////

	UFUNCTION(Server, Reliable)
		void Jumping();
	UFUNCTION(NetMulticast, Reliable)
		void JumpingOnWall();

/////////////////////////////////////WallRun//////////////////////////////////////

	UFUNCTION()
		void WallRunningBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void WallRunningEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void WallRunningStart(AActor*& OtherActor);
	UFUNCTION()
		void WallRunningStop(AActor*& OtherActor);

	UFUNCTION()
		void WallRunningCameraTiltRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void WallRunningCameraTiltLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndTiltOnWallRunning(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

/////////////////////////////////////Dash/////////////////////////////////////////

	UFUNCTION(Server, Reliable)
		void Dash();
	UFUNCTION(Server, Reliable)
		void RightDash();
	UFUNCTION(Server, Reliable)
		void ForwardDash();
	UFUNCTION(Server, Reliable)
		void CheckAndReloadDash();

//public C++ variables
public:

	UPROPERTY(Replicated)
		float SpeedBooster = 1.0f;

	UPROPERTY(BlueprintReadWrite)
		int32 JumpCounter = 0;
	
//public BP variables
public:

	UPROPERTY(BlueprintReadWrite, Category = "Character")
		AKT_PlayerCharacter* PlayerCharacter = nullptr;

/////////////////////////////////////Sprinting/////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Sprinting", meta = (ClampMin = "1.5", ClampMax = "5"))
		float SprintSpeedModifier = 1.5f;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character | Sprinting")
		bool IsSprinted = false;

/////////////////////////////////////Moving/////////////////////////////////////////
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character | Moving")
		float MovingForwardSpeed = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character | Moving")
		float MovingRightSpeed = 0;
	
/////////////////////////////////////Crouching/////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Character | Crouching")
		UCurveFloat* CurveFloatForCrouching;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.2", ClampMax = "1"), Category = "Crouching")
		float CrouchingSpeedModifier = 0.5f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character | Crouching")
		bool bCrouched = false;
	
	UPROPERTY(EditAnywhere, Category = "Character | Crouching")
		float UpperHalfHeightCapsuleCollision;
	UPROPERTY(EditAnywhere, Category = "Character | Crouching")
		float LowerHalfHeightCapsuleCollision;

/////////////////////////////////////Sliding/////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Character | Sliding")
		UCurveFloat* CurveFloatForSliding;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Sliding")
		float SlidingSpeed = 1000;

/////////////////////////////////////Jumping/////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Character | Jumping")
		int32 MaxJump = 0;

/////////////////////////////////////WallRunning/////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Character | WallRunning")
		UCurveFloat* CurveFloatForWallRunning;

	UPROPERTY(EditAnywhere, Category = "Character | WallRunning")
		UCurveFloat* CurveFloatForWallRunningCameraTilt;

	UPROPERTY(EditDefaultsOnly, Category = "Character | WallRunning")
		float TiltAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Character | WallRunning")
		float WallRunningForce;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character | WallRunning")
		FName ParkourTag;

/////////////////////////////////////Dashing/////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Character | Dashing")
		float DashSpeed = 2000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Dashing", meta = (ClampMin = "1", MaxClamp = "5"))
		int32 MaxNumberOfDashes = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Dashing", meta = (ClampMin = "1"))
		int32 StartNumberOfDashes = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Dashing", meta = (ClampMin = "1", MaxClamp = "60"))
		float DashRecoveryTime;

	UPROPERTY(EditAnywhere, Category = "Character | Dashing")
		UCurveFloat* CurveFloatForDashRecover;
};
