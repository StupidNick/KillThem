#include "Character/KT_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/KT_HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/OutputDeviceNull.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"



////////////////////////////////////////////////////Begin Play//////////////////////////////////////////////////////////

AKT_PlayerCharacter::AKT_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	ParkourCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("ParkourCapsuleComponent");
	WallRunRightCollisionComponent = CreateDefaultSubobject<UBoxComponent>("WallRunRightCollisionComponent");
	WallRunLeftCollisionComponent = CreateDefaultSubobject<UBoxComponent>("WallRunLeftCollisionComponent");
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	HealthComponent = CreateDefaultSubobject<UKT_HealthComponent>("HealthComponent");

	ParkourCapsuleComponent->SetupAttachment(GetCapsuleComponent());
	WallRunLeftCollisionComponent->SetupAttachment(GetCapsuleComponent());
	WallRunRightCollisionComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonMeshComponent->SetupAttachment(CameraComponent);
	
	GetCharacterMovement()->bWantsToCrouch = true;

	ParkourCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AKT_PlayerCharacter::WallRunningBegin);
	ParkourCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AKT_PlayerCharacter::WallRunningEnd);

	WallRunRightCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKT_PlayerCharacter::WallRunningCameraTiltRight);
	WallRunLeftCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKT_PlayerCharacter::WallRunningCameraTiltLeft);

	WallRunRightCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AKT_PlayerCharacter::EndTiltOnWallRunning);
	WallRunLeftCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AKT_PlayerCharacter::EndTiltOnWallRunning);
	
	
	CrouchingTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchingTimeline"));
	SlidingTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("SlidingTimeline"));
	WallRunningTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("WallRunningTimeline"));
	TiltCameraOnWallRunningTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TiltCameraOnWallRunningTimeLine"));

	SlidingInterpFunction.BindUFunction(this, FName("SlidingTimeLineFloatReturn"));
	CrouchingInterpFunction.BindUFunction(this, FName("CrouchingTimeLineFloatReturn"));
	WallRunningInterpFunction.BindUFunction(this, FName("WallRunningTimeLineFloatReturn"));
	TiltCameraOnWallRunningInterpFunction.BindUFunction(this, FName("TiltCameraOnWallRunningTimeLineFloatReturn"));
}


void AKT_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SprintSpeed = WalkSpeed * 1.2;

	GetMovementComponent()->SetPlaneConstraintEnabled(true);

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
		TiltCameraOnWallRunningTimeLine->AddInterpFloat(CurveFloatForWallRunningCameraTilt, TiltCameraOnWallRunningInterpFunction, FName("Alpha"));
		TiltCameraOnWallRunningTimeLine->SetLooping(false);
	}
}

void AKT_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AKT_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AKT_PlayerCharacter::Jumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AKT_PlayerCharacter::Dash);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AKT_PlayerCharacter::DoSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AKT_PlayerCharacter::DoCrouch);

	PlayerInputComponent->BindAxis("MoveForward", this, &AKT_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKT_PlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////Moving///////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::MoveForward(float Value)
{
	
	if (Value != 0.0f)
	{
		if (!OnWall)
		{
			AddMovementInput(GetActorForwardVector(), Value);
			MoveForwardValue = Value;
		}
		IsMoveForward = true;
		SetMoveForwardOnServer(true);
	}
	else
	{
		BreakSprint();
		IsMoveForward = false;
		SetMoveForwardOnServer(false);
	}
}


void AKT_PlayerCharacter::SetMoveForwardOnServer_Implementation(bool Value)
{
	if (Value)
	{
		IsMoveForward = true;
	}
	else
	{
		IsMoveForward = false;
	}
}


void AKT_PlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
		MoveRightValue = Value;
		IsMoveRight = true;
	}
	else
	{
		IsMoveRight = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////Sprint////////////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::DoSprint()
{
	if (IsMoveForward)
	{
		if (IsSprinted)
		{
			Sprint();
		}
		else
		{
			UnSprint();
		}
	}
}


void AKT_PlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SprintOnServer(WalkSpeed);
	IsSprinted = false;
	CanSliding = false;
}


void AKT_PlayerCharacter::UnSprint()
{
	if (IsCrouching)
	{
		DoCrouch();
	}
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	SprintOnServer(SprintSpeed);
	IsSprinted = true;

	FTimerHandle LTimerHandle;
	FTimerDelegate LTimerDelegate;

	LTimerDelegate.BindUFunction(this, "SlidingReload");
	GetWorldTimerManager().SetTimer(LTimerHandle, LTimerDelegate, 0.5, false);
}


void AKT_PlayerCharacter::BreakSprint()
{
	if (IsCrouching)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SprintOnServer(WalkSpeed);
	IsSprinted = false;
	CanSliding = false;
}


void AKT_PlayerCharacter::SprintOnServer_Implementation(float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////Crouching//////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::DoCrouch()
{
	if (!IsCrouching)
	{
		Crouching();
		CrouchingOnServer(true);
	}
	else
	{
		UnCrouching();
		CrouchingOnServer(false);
	}
	if (CanSliding && !GetMovementComponent()->IsFalling())
	{
		Sliding();
		SlidingOnServer();
	}
}

void AKT_PlayerCharacter::Crouching()
{
	CrouchingEndLocation = CrouchingStartLocation = CameraComponent->GetRelativeLocation();
	CrouchingEndLocation.Z -= 50;
	
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	
	CrouchingTimeLine->Play();
	
	IsCrouching = true;
	IsSprinted = false;
	CanDash = false;
}

void AKT_PlayerCharacter::UnCrouching()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	CrouchingTimeLine->Reverse();
	
	IsCrouching = false;
	CanSliding = false;
}


void AKT_PlayerCharacter::CrouchingOnServer_Implementation(bool InCrouching)
{
	if (InCrouching)
	{
		Crouching();
	}
	else
	{
		UnCrouching();
	}
}


void AKT_PlayerCharacter::CrouchingTimeLineFloatReturn_Implementation(float Value)
{
	CameraComponent->SetRelativeLocation(FMath::Lerp(CrouchingStartLocation, CrouchingEndLocation, Value));
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(UpperHalfHeightCapsuleCollision, LowerHalfHeightCapsuleCollision, Value), true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////Sliding////////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::Sliding()
{
	SlidingTimeLine->PlayFromStart();
	IsSprinted = false;
	CanSliding = false;
}

void AKT_PlayerCharacter::SlidingOnServer_Implementation()
{
	Sliding();
}


void AKT_PlayerCharacter::SlidingReload()
{
	if (IsSprinted)
	{
		CanSliding = true;
	}
}


void AKT_PlayerCharacter::SlidingTimeLineFloatReturn(float Value)
{
	FVector LSlideDirection = GetVelocity().GetSafeNormal(0.0001);
	AddActorWorldOffset(LSlideDirection * Value * UGameplayStatics::GetWorldDeltaSeconds(this) * SlidingSpeed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////Jumping/////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::Jumping()
{
	if (OnWall)
	{
		JumpingOnWall();
		JumpingOnWallOnServer();
	}
	if (IsCrouching)
	{
		DoCrouch();
	}
	if (JumpCounter < MaxJump)
	{
		LaunchCharacter(FVector(0, 0, 420), false, true);
		JumpingOnServer();
		JumpCounter++;
	}
}

void AKT_PlayerCharacter::JumpingOnWall()
{
	GetCharacterMovement()->GravityScale = 1;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 0, 0));
	OnWall = false;
}

void AKT_PlayerCharacter::JumpingOnWallOnServer_Implementation()
{
	JumpingOnWall();
}


void AKT_PlayerCharacter::JumpingOnServer_Implementation()
{
	LaunchCharacter(FVector(0, 0, 420), false, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////Dashing/////////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::Dash()
{
	if (CanDash)
	{
		if (IsMoveForward)
		{
			if (MoveForwardValue >= 0)
			{
				ForwardDash(true);
				DashOnServer(true, true);
			}
			else
			{
				ForwardDash(false);
				DashOnServer(true, false);
			}
		}
		else if (IsMoveRight)
		{
			if (MoveRightValue > 0)
			{
				RightDash(true);
				DashOnServer(false, true);
			}
			else
			{
				RightDash(false);
				DashOnServer(false, false);
			}
		}
		
		CanDash = false;

		FTimerHandle LTimerHandle;
		FTimerDelegate LTimerDelegate;

		LTimerDelegate.BindUFunction(this, "DashReload");
		GetWorldTimerManager().SetTimer(LTimerHandle, LTimerDelegate, 2, false);
	}
}


void AKT_PlayerCharacter::RightDash(bool Right)
{
	if (Right)
	{
		LaunchCharacter(FVector(UKismetMathLibrary::GetRightVector(GetActorRotation()) * DashSpeed), true, false);
	}
	else
	{
		LaunchCharacter(FVector(UKismetMathLibrary::GetRightVector(GetActorRotation()) * -DashSpeed), true, false);
	}
}


void AKT_PlayerCharacter::ForwardDash(bool Forward)
{
	if (Forward)
	{
		LaunchCharacter(FVector(UKismetMathLibrary::GetForwardVector(GetActorRotation()) * DashSpeed), true, false);
	}
	else
	{
		LaunchCharacter(FVector(UKismetMathLibrary::GetForwardVector(GetActorRotation()) * -DashSpeed), true, false);
	}
}


void AKT_PlayerCharacter::DashOnServer_Implementation(bool MoveForward, bool Value)
{
	if (MoveForward)
	{
		if (Value)
		{
			ForwardDash(true);
		}
		else
		{
			ForwardDash(false);
		}
	}
	else
	{
		if (Value)
		{
			RightDash(true);
		}
		else
		{
			RightDash(false);
		}
	}
}


void AKT_PlayerCharacter::DashReload()
{
	CanDash = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////WallRunning//////////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::WallRunningBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WallRunningStart(OtherActor);
	WallRunningBeginOnServer(OtherActor);
}


void AKT_PlayerCharacter::WallRunningEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	WallRunningStop(OtherActor);
	WallRunningEndOnServer(OtherActor);
}

void AKT_PlayerCharacter::WallRunningStart(AActor* OtherActor)
{
	PlayerDirectionForWallRunning = CameraComponent->GetForwardVector();
	
	if (OtherActor->ActorHasTag(ParkourTag) && GetMovementComponent()->IsFalling())
	{
		OnWall = true;
		JumpCounter = 0;
		WallRunningTimeLine->PlayFromStart();
	}
}

void AKT_PlayerCharacter::WallRunningStop(AActor* OtherActor)
{
	WallRunningTimeLine->Stop();

	if (OtherActor->ActorHasTag(ParkourTag))
	{
		GetCharacterMovement()->GravityScale = 1;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 0, 0));
		OnWall = false;
	}
}


void AKT_PlayerCharacter::WallRunningBeginOnServer_Implementation(AActor* OtherActor)
{
	WallRunningStart(OtherActor);
}

void AKT_PlayerCharacter::WallRunningEndOnServer_Implementation(AActor* OtherActor)
{
	WallRunningStop(OtherActor);
}


void AKT_PlayerCharacter::WallRunningTimeLineFloatReturn_Implementation(float Value)
{
	if (IsMoveForward)
	{
		if (OnWall)
		{
			GetCharacterMovement()->GravityScale = 0;
			GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 0, 1));
			AddMovementInput(PlayerDirectionForWallRunning, WallRunningForce);
		}
		else
		{
			GetCharacterMovement()->GravityScale = 1;
			GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 0, 0));
			OnWall = false;
		}
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1;
		GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 0, 0));
		OnWall = false;
	}
}


void AKT_PlayerCharacter::TiltCameraOnWallRunningTimeLineFloatReturn(float Value)
{
	if (CameraTiltToRight)
	{
		if (PlayerController)
		{
			PlayerController->SetControlRotation(FMath::LerpRange(GetActorRotation(), FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, -TiltAngle), Value));
		}
	}
	else
	{
		if (PlayerController)
		{
			PlayerController->SetControlRotation(FMath::LerpRange(GetActorRotation(), FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, TiltAngle), Value));
		}
	}
}


void AKT_PlayerCharacter::WallRunningCameraTiltRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(ParkourTag) && GetMovementComponent()->IsFalling())
	{
		CameraTiltToRight = true;
		CameraIsTilt = true;
		TiltCameraOnWallRunningTimeLine->PlayFromStart();
	}
}


void AKT_PlayerCharacter::WallRunningCameraTiltLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(ParkourTag) && GetMovementComponent()->IsFalling())
	{
		CameraTiltToRight = false;
		CameraIsTilt = true;
		TiltCameraOnWallRunningTimeLine->PlayFromStart();
	}
}


void AKT_PlayerCharacter::EndTiltOnWallRunning(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CameraIsTilt)
	{
		TiltCameraOnWallRunningTimeLine->ReverseFromEnd();
		CameraIsTilt = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////