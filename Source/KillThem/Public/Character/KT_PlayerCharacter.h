#pragma once

#include "CoreMinimal.h"


#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"


#include "KT_PlayerCharacter.generated.h"



class AKT_BaseInteractiveObject;
class UKT_ItemsManagerComponent;
class UBoxComponent;
class UCameraComponent;
class UMovementComponent;
class UCurveFloat;
class UKT_HealthComponent;
class AKT_BaseWeapon;



UCLASS()
class KILLTHEM_API AKT_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

//Constructor
public:
	
	AKT_PlayerCharacter();

//private с++ functions
private:

	UFUNCTION()
		void SlidingTimeLineFloatReturn(float Value);
	
	UFUNCTION(NetMulticast, Reliable)
		void CrouchingTimeLineFloatReturn(float Value);

	UFUNCTION(NetMulticast, Reliable)
		void WallRunningTimeLineFloatReturn(float Value);

	UFUNCTION()
		void TiltCameraOnWallRunningTimeLineFloatReturn(float Value);

//private с++ variables
private:

	UTimelineComponent* SlidingTimeLine;

	UTimelineComponent* CrouchingTimeLine;

	UTimelineComponent* WallRunningTimeLine;

	UTimelineComponent* TiltCameraOnWallRunningTimeLine;

	UPROPERTY()
		FVector CrouchingStartLocation;
	UPROPERTY()
		FVector CrouchingEndLocation;

	UPROPERTY()
		bool CameraTiltToRight;

	UPROPERTY()
		bool CameraIsTilt = false;

	FTimerHandle SprintTimerHandle;
	FTimerDelegate SprintTimerDelegate;


//protected c++ functions
protected:
	
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	UFUNCTION(Server, Reliable)
		void SetMoveForwardOnServer(bool Value);
	void MoveRight(float Value);

/////////////////////////////////////Sprint/////////////////////////////////////////
	UFUNCTION()
		void DoSprint();
	UFUNCTION()
		void Sprint();
	UFUNCTION()
		void UnSprint();
	UFUNCTION()
		void BreakSprint();
	UFUNCTION(Server, Reliable)
		void SetCharacterSpeedOnServer(const float InSpeed);
	UFUNCTION(Server, Reliable)
		void SetCanShootOnServer(const bool InCanShoot);

/////////////////////////////////////Crouch/////////////////////////////////////////
	UFUNCTION()
		void DoCrouch();
	UFUNCTION()
		void Crouching();
	UFUNCTION()
		void UnCrouching();
	UFUNCTION(Server, Reliable)
		void CrouchingOnServer(bool InCrouching);

/////////////////////////////////////Slide/////////////////////////////////////////
	UFUNCTION()
		void Sliding();
	UFUNCTION()
		void SlidingReload();
	UFUNCTION(Server, Reliable)
		void SlidingOnServer();

/////////////////////////////////////Jump/////////////////////////////////////////
	UFUNCTION()
		void Jumping();
	UFUNCTION()
		void JumpingOnWall();
	UFUNCTION(Server, Reliable)
		void JumpingOnWallOnServer();
	UFUNCTION(Server, Reliable)
		void JumpingOnServer();

/////////////////////////////////////Dash/////////////////////////////////////////
	UFUNCTION()
		void Dash();
	UFUNCTION()
		void RightDash(bool Right);
	UFUNCTION()
		void ForwardDash(bool Forward);
	UFUNCTION(Server, Reliable)
		void DashOnServer(bool MoveForward, bool Value);
	UFUNCTION()
		void DashReload();

/////////////////////////////////////WallRun//////////////////////////////////////

	UFUNCTION()
		void WallRunningBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void WallRunningEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void WallRunningStart(AActor* OtherActor);
	UFUNCTION()
		void WallRunningStop(AActor* OtherActor);


	UFUNCTION()
		void WallRunningCameraTiltRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void WallRunningCameraTiltLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndTiltOnWallRunning(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

/////////////////////////////////////Weapon////////////////////////////////////////
	
	UFUNCTION()
		void OnChangeWeaponPressed();
	
	UFUNCTION(Server, Reliable)
		void ChangeWeaponOnServer();
	
	UFUNCTION(Server, Reliable)
		void FireOnServer();

	UFUNCTION(Server, Reliable)
		void StopFireOnServer();

	UFUNCTION(Server, Reliable)
		void ReloadOnServer();

	UFUNCTION(Server, Reliable)
		void RightClick();

	UFUNCTION(Server, Reliable)
		void RightUnClick();

	UFUNCTION(Server, Reliable)
		void AlterFireOnServer();

	UFUNCTION()
		void Scope();

	UFUNCTION()
		void UnScope();

/////////////////////////////////////Interact//////////////////////////////////////

	UFUNCTION(Server, Reliable)
		void InteractOnServer(AKT_BaseInteractiveObject* InInteractiveObject);
	UFUNCTION()
		void Interact();

	
//protected c++ variables
protected:

	UPROPERTY(BlueprintReadWrite)
		int JumpCounter = 0;

	UPROPERTY()
		FVector PlayerDirectionForWallRunning;

	UPROPERTY()
		bool CanInteract;

	UPROPERTY()
		AKT_BaseInteractiveObject* InteractiveObject = nullptr;
	

//public c++ functions
public:

	UFUNCTION(Server, Reliable)
		void AddWeapon(TSubclassOf<AKT_BaseWeapon> InWeaponClass, const int InAmountOfAmmo, const int AmmoInTheClip = -1);

	UFUNCTION()
		void InteractInfo(AKT_BaseInteractiveObject* InInteractiveObject);

	UFUNCTION()
		void UnInteractInfo();

	UFUNCTION()
		void ChangeCharacterSpeeds(const float InSpeedFactor);

	UFUNCTION(Server, Reliable)
		void CheckCanFireOnServer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//public c++ variables
public:

/////////////////////////////////////Timelines/////////////////////////////////////////
	FOnTimelineFloat SlidingInterpFunction{};

	FOnTimelineFloat CrouchingInterpFunction{};

	FOnTimelineFloat WallRunningInterpFunction{};

	FOnTimelineFloat TiltCameraOnWallRunningInterpFunction{};
//////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY()
		bool CanShoot = true;
	
	UPROPERTY()
		bool NeedShoot = false;

	
////////////////////////////////////PersonParams//////////////////////////////////////

	UPROPERTY()
		float SpeedOfWalk;

	UPROPERTY()
		float SpeedOfRun;

	UPROPERTY()
		float SpeedOfCrouch;

	UPROPERTY()
		float SpeedOfSliding;

	UPROPERTY()
		float SpeedOfDash;

	UPROPERTY()
		float DamageBooster;

	UPROPERTY()
		float BerserkBooster;

//////////////////////////////////////////////////////////////////////////////////////


//public BP variables
public:

	UPROPERTY(EditAnywhere, Category = "Character | Components")
		UKT_ItemsManagerComponent* ItemsManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Components")
		UKT_HealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Components")
		USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		UCapsuleComponent* ParkourCapsuleComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		UBoxComponent* WallRunRightCollisionComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		UBoxComponent* WallRunLeftCollisionComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Parkour")
		FName ParkourTag;

/////////////////////////////////////Moving/////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Character | Moving")
		bool IsMoveForward = false;
	UPROPERTY(BlueprintReadOnly, Category = "Character | Moving")
		bool IsMoveRight = false;
	UPROPERTY(BlueprintReadOnly, Category = "Character | Moving")
		float MoveForwardValue = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Character | Moving")
		float MoveRightValue = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Moving")
		float WalkSpeed = 600;

/////////////////////////////////////Sprinting/////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Character | Sprinting")
		bool IsSprinted = false;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Sprinting")
		float SprintSpeed = 1200;
	
/////////////////////////////////////Crouching/////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Character | Crouching")
		UCurveFloat* CurveFloatForCrouching;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Crouching")
		float CrouchSpeed = 300;
	UPROPERTY(BlueprintReadOnly, Category = "Character | Crouching")
		bool IsCrouching = false;
	UPROPERTY(EditAnywhere, Category = "Character | Crouching")
		float UpperHalfHeightCapsuleCollision;
	UPROPERTY(EditAnywhere, Category = "Character | Crouching")
		float LowerHalfHeightCapsuleCollision;

/////////////////////////////////////Sliding/////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Character | Sliding")
		UCurveFloat* CurveFloatForSliding;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Sliding")
		float SlidingSpeed = 1000;
	UPROPERTY(BlueprintReadOnly, Category = "Character | Sliding")
		bool CanSliding = false;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Jumping")
		int MaxJump = 0;

/////////////////////////////////////Dashing/////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Character | Dashing")
		bool CanDash = true;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Dashing")
		float DashSpeed = 2000;

/////////////////////////////////////WallRunning/////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Character | WallRunning")
		UCurveFloat* CurveFloatForWallRunning;

	UPROPERTY(EditAnywhere, Category = "Character | WallRunning")
		UCurveFloat* CurveFloatForWallRunningCameraTilt;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character | WallRunning")
		bool OnWall;

	UPROPERTY(EditDefaultsOnly, Category = "Character | WallRunning")
		float TiltAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Character | WallRunning")
		float WallRunningForce;
};



