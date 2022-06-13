#include "Components/KT_CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"


UKT_CharacterMovementComponent::UKT_CharacterMovementComponent()
{
	CrouchingTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchingTimeline"));
	SlidingTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("SlidingTimeline"));
	WallRunningTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("WallRunningTimeline"));
	TiltCameraOnWallRunningTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TiltCameraOnWallRunningTimeLine"));
	DashRecoverTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("DashRecoverTimeLine"));

	CrouchingInterpFunction.BindUFunction(this, FName("CrouchingTimeLineFloatReturn"));
	SlidingInterpFunction.BindUFunction(this, FName("SlidingTimeLineFloatReturn"));
	WallRunningInterpFunction.BindUFunction(this, FName("WallRunningTimeLineFloatReturn"));
	TiltCameraOnWallRunningInterpFunction.BindUFunction(this, FName("TiltCameraOnWallRunningTimeLineFloatReturn"));
	DashRecoveringInterpFunction.BindUFunction(this, FName("DashRecoveringTimeLineFloatReturn"));
}


void UKT_CharacterMovementComponent::Initialize()
{
	if (CurveFloatForSliding)
		{
		SlidingTimeLine->AddInterpFloat(CurveFloatForSliding, SlidingInterpFunction, FName("Alpha"));
		SlidingTimeLine->SetLooping(false);
		}
	if (CurveFloatForCrouching)
	{
		CrouchingTimeLine->AddInterpFloat(CurveFloatForCrouching, CrouchingInterpFunction, FName("Alpha"));
		CrouchingTimeLine->SetLooping(false);
	}
	if (CurveFloatForWallRunning)
	{
		WallRunningTimeLine->AddInterpFloat(CurveFloatForWallRunning, WallRunningInterpFunction, FName("Alpha"));
		WallRunningTimeLine->SetLooping(true);
	}
	if (CurveFloatForWallRunningCameraTilt)
	{
		TiltCameraOnWallRunningTimeLine->AddInterpFloat(CurveFloatForWallRunningCameraTilt,
														TiltCameraOnWallRunningInterpFunction, FName("Alpha"));
		TiltCameraOnWallRunningTimeLine->SetLooping(false);
	}
	//Dash initialize
	if (CurveFloatForDashRecover)
	{
		FOnTimelineEventStatic OnTimelineFinishedCallback;
		DashRecoverTimeLine->AddInterpFloat(CurveFloatForDashRecover,
														DashRecoveringInterpFunction, FName("Alpha"));
		OnTimelineFinishedCallback.BindUFunction(this, FName("DashRecoveryFinished"));
		DashRecoverTimeLine->SetTimelineFinishedFunc(OnTimelineFinishedCallback);
		DashRecoverTimeLine->SetLooping(false);
	}

	DashCounter = StartNumberOfDashes;
	CrouchingEndLocation = CrouchingStartLocation = PlayerCharacter->CameraComponent->GetRelativeLocation();
	CrouchingEndLocation.Z -= UpperHalfHeightCapsuleCollision - LowerHalfHeightCapsuleCollision;
	SetPlaneConstraintEnabled(true);
}


void UKT_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AKT_PlayerCharacter>(GetPawnOwner());

	check(PlayerCharacter);

	Initialize();
}


void UKT_CharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKT_CharacterMovementComponent, IsSprinted);
	DOREPLIFETIME(UKT_CharacterMovementComponent, bCrouched);
	DOREPLIFETIME(UKT_CharacterMovementComponent, DashCounter);
	DOREPLIFETIME(UKT_CharacterMovementComponent, DashRecoveryCounter);
	DOREPLIFETIME(UKT_CharacterMovementComponent, MovingForwardSpeed);
	DOREPLIFETIME(UKT_CharacterMovementComponent, MovingRightSpeed);
	DOREPLIFETIME(UKT_CharacterMovementComponent, SpeedBooster);
	DOREPLIFETIME(UKT_CharacterMovementComponent, JumpCounter);
}


float UKT_CharacterMovementComponent::GetMaxSpeed() const
{
	const float LMaxSpeed = Super::GetMaxSpeed() * SpeedBooster;
	if (!IsValid(PlayerCharacter)) return LMaxSpeed;

	if (IsSprinting())
	{
		return LMaxSpeed * SprintSpeedModifier;
	}
	if (bCrouched)
	{
		return LMaxSpeed * CrouchingSpeedModifier;
	}
	return LMaxSpeed;
}


///////////////////////////////////////////////////Moving///////////////////////////////////////////////////////////////

void UKT_CharacterMovementComponent::MoveForward_Implementation(float Value)
{
	if (Value != 0.0f)
	{
		if (!OnWall)
		{
			PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), Value);
		}
	}
	else
	{
		if (IsSprinted)
		{
			UnSprint();
		}
	}
	SetMoveForwardOnServer(Value);
}


void UKT_CharacterMovementComponent::SetMoveForwardOnServer_Implementation(const float& Value)
{
	MovingForwardSpeed = Value;
}


void UKT_CharacterMovementComponent::MoveRight_Implementation(float Value)
{
	if (Value != 0.0f)
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), Value);

	SetMoveRightOnServer(Value);
}


void UKT_CharacterMovementComponent::SetMoveRightOnServer_Implementation(const float& Value)
{
	MovingRightSpeed = Value;
}


bool UKT_CharacterMovementComponent::GetIsMovingForward() const
{
	if (MovingForwardSpeed > 0) return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////Sprinting//////////////////////////////////////////////////////////////

void UKT_CharacterMovementComponent::ChangeSprint_Implementation()
{
	if (!GetIsMovingForward()) return;
		
	if (IsSprinted)
	{
		UnSprint();
	}
	else
	{
		Sprint();
	}
}


void UKT_CharacterMovementComponent::Sprint_Implementation()
{
	PlayerCharacter->ItemsManagerComponent->SetCanShoot(false);
	IsSprinted = true;
	UnCrouching();
}


void UKT_CharacterMovementComponent::UnSprint_Implementation()
{
	PlayerCharacter->ItemsManagerComponent->SetCanShoot(true);
	IsSprinted = false;
}


bool UKT_CharacterMovementComponent::IsSprinting() const 
{
	if (!IsValid(PlayerCharacter)) return false;
	
	return IsSprinted && GetIsMovingForward() && PlayerCharacter->GetVelocity().Size() > 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////Crouching//////////////////////////////////////////////////////////////

void UKT_CharacterMovementComponent::ChangeCrouch_Implementation()
{
	if (GetCanSliding() && !IsFalling())
	{
		Sliding();
	}
	if (!bCrouched)
	{
		Crouching();
	}
	else
	{
		UnCrouching();
	}
}

void UKT_CharacterMovementComponent::Crouching_Implementation()
{
	CrouchingTimeLine->Play();
	
	bCrouched = true;
	IsSprinted = false;
	PlayerCharacter->ItemsManagerComponent->SetCanShoot(true);
	// CanDash = false;
}

void UKT_CharacterMovementComponent::UnCrouching_Implementation()
{
	CrouchingTimeLine->Reverse();
	
	bCrouched = false;
}


void UKT_CharacterMovementComponent::CrouchingTimeLineFloatReturn_Implementation(float Value)
{
	PlayerCharacter->CameraComponent->SetRelativeLocation(FMath::Lerp(CrouchingStartLocation, CrouchingEndLocation, Value));
	PlayerCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(
		FMath::Lerp(UpperHalfHeightCapsuleCollision, LowerHalfHeightCapsuleCollision, Value), true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////Sliding////////////////////////////////////////////////////////////////

void UKT_CharacterMovementComponent::Sliding_Implementation()
{
	SlidingTimeLine->PlayFromStart();
	IsSprinted = false;
}


bool UKT_CharacterMovementComponent::GetCanSliding() const
{
	FVector LVector;
	float LCharacterSpeed;
	PlayerCharacter->GetVelocity().ToDirectionAndLength(LVector, LCharacterSpeed);
	if (!IsSprinted || LCharacterSpeed < GetMaxSpeed() - 10) return false;

	return true;
}


void UKT_CharacterMovementComponent::SlidingTimeLineFloatReturn_Implementation(float Value)
{
	const FVector LSlideDirection = PlayerCharacter->GetVelocity().GetSafeNormal(0.0001);
	PlayerCharacter->AddActorWorldOffset(LSlideDirection * Value * UGameplayStatics::GetWorldDeltaSeconds(this) * SlidingSpeed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////Jumping/////////////////////////////////////////////////////////////

void UKT_CharacterMovementComponent::Jumping_Implementation()
{
	if (OnWall)
	{
		JumpingOnWall();
	}
	if (bCrouched)
	{
		ChangeCrouch();
		return;
	}
	if (JumpCounter < MaxJump)
	{
		PlayerCharacter->LaunchCharacter(FVector(0, 0, 420), false, true);
		JumpCounter++;
	}
}

void UKT_CharacterMovementComponent::JumpingOnWall_Implementation()
{
	GravityScale = 1;
	SetPlaneConstraintNormal(FVector(0, 0, 0));
	OnWall = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////WallRunning//////////////////////////////////////////////////////////////////

void UKT_CharacterMovementComponent::WallRunningBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	WallRunningStart(OtherActor);
}


void UKT_CharacterMovementComponent::WallRunningEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	WallRunningStop(OtherActor);
}

void UKT_CharacterMovementComponent::WallRunningStart(AActor*& OtherActor)
{
	if (!IsValid(PlayerCharacter) || !IsValid(OtherActor)) return;
	
	PlayerDirectionForWallRunning = PlayerCharacter->CameraComponent->GetForwardVector();

	if (OtherActor->ActorHasTag(ParkourTag) && IsFalling())
	{
		OnWall = true;
		JumpCounter = 0;
		WallRunningTimeLine->PlayFromStart();
	}
}

void UKT_CharacterMovementComponent::WallRunningStop(AActor*& OtherActor)
{
	WallRunningTimeLine->Stop();

	if (OtherActor->ActorHasTag(ParkourTag))
	{
		GravityScale = 1;
		SetPlaneConstraintNormal(FVector(0, 0, 0));
		OnWall = false;
	}
}


void UKT_CharacterMovementComponent::WallRunningTimeLineFloatReturn_Implementation(float Value)
{
	if (GetIsMovingForward())
	{
		if (OnWall)
		{
			GravityScale = 0;
			SetPlaneConstraintNormal(FVector(0, 0, 1));
			PlayerCharacter->AddMovementInput(PlayerDirectionForWallRunning, WallRunningForce);
		}
		else
		{
			GravityScale = 1;
			SetPlaneConstraintNormal(FVector(0, 0, 0));
			OnWall = false;
		}
	}
	else
	{
		GravityScale = 1;
		SetPlaneConstraintNormal(FVector(0, 0, 0));
		OnWall = false;
	}
}


void UKT_CharacterMovementComponent::TiltCameraOnWallRunningTimeLineFloatReturn(float Value)
{
	if (CameraTiltToRight)
	{
		if (PlayerCharacter->PlayerController)
		{
			PlayerCharacter->PlayerController->SetControlRotation(FMath::LerpRange(PlayerCharacter->GetActorRotation(),
			                                                      FRotator(PlayerCharacter->GetActorRotation().Pitch,
			                                                               PlayerCharacter->GetActorRotation().Yaw, -TiltAngle), Value));
		}
	}
	else
	{
		if (PlayerCharacter->PlayerController)
		{
			PlayerCharacter->PlayerController->SetControlRotation(FMath::LerpRange(PlayerCharacter->GetActorRotation(),
			                                                      FRotator(PlayerCharacter->GetActorRotation().Pitch,
			                                                               PlayerCharacter->GetActorRotation().Yaw, TiltAngle), Value));
		}
	}
}


void UKT_CharacterMovementComponent::WallRunningCameraTiltRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                                bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(ParkourTag) && IsFalling())
	{
		CameraTiltToRight = true;
		CameraIsTilt = true;
		TiltCameraOnWallRunningTimeLine->PlayFromStart();
	}
}


void UKT_CharacterMovementComponent::WallRunningCameraTiltLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(ParkourTag) && IsFalling())
	{
		CameraTiltToRight = false;
		CameraIsTilt = true;
		TiltCameraOnWallRunningTimeLine->PlayFromStart();
	}
}


void UKT_CharacterMovementComponent::EndTiltOnWallRunning(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CameraIsTilt)
	{
		TiltCameraOnWallRunningTimeLine->ReverseFromEnd();
		CameraIsTilt = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////Dashing/////////////////////////////////////////////////////////////////

void UKT_CharacterMovementComponent::Dash_Implementation()
{
	if (!GetCanDash()) return;
	
	if (MovingRightSpeed != 0)
	{
		RightDash();
	}
	else
	{
		ForwardDash();
	}
	DashCounter--;

	DashUsed();
	CheckAndReloadDash();
}


bool UKT_CharacterMovementComponent::GetCanDash() const
{
	if (DashCounter > 0) return true;
	return false;
}


void UKT_CharacterMovementComponent::RightDash_Implementation()
{
	if (MovingRightSpeed > 0)
	{
		PlayerCharacter->LaunchCharacter(FVector(UKismetMathLibrary::GetRightVector(PlayerCharacter->GetActorRotation()) * DashSpeed), true, false);
	}
	else
	{
		PlayerCharacter->LaunchCharacter(FVector(UKismetMathLibrary::GetRightVector(PlayerCharacter->GetActorRotation()) * -DashSpeed), true, false);
	}
}


void UKT_CharacterMovementComponent::ForwardDash_Implementation()
{
	if (MovingForwardSpeed >= 0)
	{
		PlayerCharacter->LaunchCharacter(FVector(UKismetMathLibrary::GetForwardVector(PlayerCharacter->GetActorRotation()) * DashSpeed), true, false);
	}
	else
	{
		PlayerCharacter->LaunchCharacter(FVector(UKismetMathLibrary::GetForwardVector(PlayerCharacter->GetActorRotation()) * -DashSpeed), true, false);
	}
}


void UKT_CharacterMovementComponent::CheckAndReloadDash_Implementation()
{
	if (!IsValid(PlayerCharacter) || DashRecoverTimeLine->IsPlaying() || DashCounter >= MaxNumberOfDashes) return;

	DashRecoveryCounter = 0;
	DashRecoveryValuePerTick = DashRecoveryTime;
	DashRecoverTimeLine->SetPlaybackPosition(DashRecoveryTime, true);
	DashRecoverTimeLine->Reverse();
}


void UKT_CharacterMovementComponent::DashRecoveringTimeLineFloatReturn_Implementation(float Value)
{
	DashRecoveryCounter += (DashRecoveryValuePerTick - Value) * SpeedBooster;
	DashRecoveryValuePerTick = Value;
	
	if (DashRecoveryCounter >= DashRecoveryTime) DashRecoverTimeLine->Stop();
}


void UKT_CharacterMovementComponent::OnRep_DashRecoveryCounterUpdate_Implementation()
{
	if (!IsValid(PlayerCharacter)) return;
	
	PlayerCharacter->OnDashRecoverUpdates.Broadcast(DashRecoveryCounter/DashRecoveryTime, DashCounter);
}


void UKT_CharacterMovementComponent::DashUsed_Implementation()
{
	if (!IsValid(PlayerCharacter)) return;
	
	PlayerCharacter->OnDashUsed.Broadcast(DashCounter);
}


void UKT_CharacterMovementComponent::DashRecoveryFinished_Implementation()
{
	DashCounter++;
	CheckAndReloadDash();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////