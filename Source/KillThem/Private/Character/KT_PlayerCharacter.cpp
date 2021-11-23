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
#include "Weapons/KT_BaseWeapon.h"
#include "InteractiveObjects/KT_BaseInteractiveObject.h"

#include "Components/KT_ItemsManagerComponent.h"
#include "Components/ProgressBar.h"
#include "Editor/EditorEngine.h"
#include "GameMode/KT_GameHUD.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"


////////////////////////////////////////////////////Begin Play//////////////////////////////////////////////////////////

AKT_PlayerCharacter::AKT_PlayerCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	ParkourCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("ParkourCapsuleComponent");
	WallRunRightCollisionComponent = CreateDefaultSubobject<UBoxComponent>("WallRunRightCollisionComponent");
	WallRunLeftCollisionComponent = CreateDefaultSubobject<UBoxComponent>("WallRunLeftCollisionComponent");
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	HealthComponent = CreateDefaultSubobject<UKT_HealthComponent>("HealthComponent");
	ItemsManagerComponent = CreateDefaultSubobject<UKT_ItemsManagerComponent>(TEXT("ItemsManagerComponent"));

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
	ScopingTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScopingTimeLine"));

	SlidingInterpFunction.BindUFunction(this, FName("SlidingTimeLineFloatReturn"));
	CrouchingInterpFunction.BindUFunction(this, FName("CrouchingTimeLineFloatReturn"));
	WallRunningInterpFunction.BindUFunction(this, FName("WallRunningTimeLineFloatReturn"));
	TiltCameraOnWallRunningInterpFunction.BindUFunction(this, FName("TiltCameraOnWallRunningTimeLineFloatReturn"));
	ScopingInterpFunction.BindUFunction(this, FName("ScopingTimeLineFloatReturn"));

	ItemsManagerComponent->Initialize(this);
	
	ItemsManagerComponent->SetIsReplicated(true);
	HealthComponent->SetIsReplicated(true);
}


void AKT_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() && IsValid(HUD))
	{
		HUD->CreateMainHUD_WD();
		ItemsManagerComponent->AmountOfAmmoChanged(ItemsManagerComponent->AmmoForFirstWeapon);
	}
	
	SprintSpeed = WalkSpeed * 1.5; //Fix this
	
	SpeedOfWalk = WalkSpeed;
	SpeedOfRun = SprintSpeed;
	SpeedOfCrouch = CrouchSpeed;
	SpeedOfSliding = SlidingSpeed;
	SpeedOfDash = DashSpeed;
	DamageBooster = 1;
	BerserkBooster = 1;

	CanShoot = true;

	SprintTimerDelegate.BindUFunction(this, "SlidingReload");

	GetMovementComponent()->SetPlaneConstraintEnabled(true);

	GetCharacterMovement()->MaxWalkSpeed = SpeedOfWalk;

	DefaultArmsTransform = FirstPersonMeshComponent->GetRelativeTransform();

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
	if (ScopingCameraTilt)
	{
		ScopingTimeLine->AddInterpFloat(ScopingCameraTilt, ScopingInterpFunction, FName("Alpha"));
		ScopingTimeLine->SetLooping(false);
	}
	if (IsValid(ItemsManagerComponent->FirstWeaponSlotClass) && HasAuthority())
	{
		AddWeapon(ItemsManagerComponent->FirstWeaponSlotClass, ItemsManagerComponent->AmmoForFirstWeapon);
	}
}


void AKT_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &AKT_PlayerCharacter::OnEscapeButtonPressed);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AKT_PlayerCharacter::Interact);
	
	PlayerInputComponent->BindAction("FirstGrenadeSlot", IE_Pressed, this, &AKT_PlayerCharacter::DropFirstGrenadeOnServer);
	
	PlayerInputComponent->BindAction("SecondGrenadeSlot", IE_Pressed, this, &AKT_PlayerCharacter::DropSecondGrenadeOnServer);
	
	PlayerInputComponent->BindAction("AlterFire", IE_Pressed, this, &AKT_PlayerCharacter::RightClick);
	PlayerInputComponent->BindAction("AlterFire", IE_Released, this, &AKT_PlayerCharacter::RightUnClick);
	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AKT_PlayerCharacter::FireOnServer);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AKT_PlayerCharacter::StopFireOnServer);
	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AKT_PlayerCharacter::ReloadOnServer);

	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AKT_PlayerCharacter::ChangeWeaponOnServer);

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


void AKT_PlayerCharacter::VisibleBooster_Implementation(UTexture2D* Icon, const float Time)
{
	if (IsValid(HUD))
	{
		HUD->MainHUD->Ability->BackgroundImage->SetBrushFromTexture(Icon);
		HUD->MainHUD->Ability->TimerTime = Time;
		HUD->MainHUD->Ability->SetVisibility(ESlateVisibility::Visible);
	}
}


void AKT_PlayerCharacter::DisableBooster_Implementation()
{
	if (IsValid(HUD))
	{
		HUD->MainHUD->Ability->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AKT_PlayerCharacter::BerserkBoostOnServer_Implementation(const float Boost)
{
	BerserkBooster *= Boost;
}


void AKT_PlayerCharacter::RageBoostOnServer_Implementation(const float Boost)
{
	SpeedOfWalk *= Boost;
	SpeedOfRun *= Boost;
	SpeedOfCrouch *= Boost;
	SpeedOfSliding *= Boost;
	SpeedOfDash *= Boost;
}


void AKT_PlayerCharacter::SpeedBoostOnServer_Implementation(const float Boost)
{
	BerserkBooster *= Boost;
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
		if (IsSprinted)
		{
			BreakSprint();
		}
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


void AKT_PlayerCharacter::ChangeCharacterSpeeds(const float InSpeedFactor)
{
	SpeedOfWalk *= InSpeedFactor;
	SpeedOfRun *= InSpeedFactor;
	SpeedOfCrouch *= InSpeedFactor;
	SpeedOfSliding *= InSpeedFactor;
	SpeedOfDash *= InSpeedFactor;

	if (IsSprinted)
	{
		GetCharacterMovement()->MaxWalkSpeed = SpeedOfRun;
		SetCharacterSpeedOnServer(SpeedOfRun);
	}
	else if (IsCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = SpeedOfCrouch;
		SetCharacterSpeedOnServer(SpeedOfCrouch);
	}
	else 
	{
		GetCharacterMovement()->MaxWalkSpeed = SpeedOfWalk;
		SetCharacterSpeedOnServer(SpeedOfWalk);
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
			UnSprint();
		}
		else
		{
			Sprint();
		}
	}
}


void AKT_PlayerCharacter::Sprint()
{
	if (IsCrouching)
	{
		DoCrouch();
	}
	GetCharacterMovement()->MaxWalkSpeed = SpeedOfRun;
	SetCharacterSpeedOnServer(SpeedOfRun);
	SetCanShootOnServer(false);
	IsSprinted = true;
	
	GetWorldTimerManager().SetTimer(SprintTimerHandle, SprintTimerDelegate, 0.5, false);
}


void AKT_PlayerCharacter::UnSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SpeedOfWalk;
	SetCharacterSpeedOnServer(SpeedOfWalk);
	SetCanShootOnServer(true);
	IsSprinted = false;
	CanSliding = false;
}


void AKT_PlayerCharacter::BreakSprint()
{
	if (IsCrouching)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = SpeedOfWalk;
	SetCharacterSpeedOnServer(SpeedOfWalk);
	SetCanShootOnServer(true);
	IsSprinted = false;
	CanSliding = false;
}


void AKT_PlayerCharacter::SetCharacterSpeedOnServer_Implementation(const float InSpeed)
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
	
	GetCharacterMovement()->MaxWalkSpeed = SpeedOfCrouch;
	
	CrouchingTimeLine->Play();

	CanShoot = true;
	IsCrouching = true;
	IsSprinted = false;
	CanDash = false;
}

void AKT_PlayerCharacter::UnCrouching()
{
	GetCharacterMovement()->MaxWalkSpeed = SpeedOfWalk;
	
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
		LaunchCharacter(FVector(UKismetMathLibrary::GetRightVector(GetActorRotation()) * SpeedOfDash), true, false);
	}
	else
	{
		LaunchCharacter(FVector(UKismetMathLibrary::GetRightVector(GetActorRotation()) * -SpeedOfDash), true, false);
	}
}


void AKT_PlayerCharacter::ForwardDash(bool Forward)
{
	if (Forward)
	{
		LaunchCharacter(FVector(UKismetMathLibrary::GetForwardVector(GetActorRotation()) * SpeedOfDash), true, false);
	}
	else
	{
		LaunchCharacter(FVector(UKismetMathLibrary::GetForwardVector(GetActorRotation()) * -SpeedOfDash), true, false);
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
}


void AKT_PlayerCharacter::WallRunningEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	WallRunningStop(OtherActor);
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


///////////////////////////////////////////Weapons//////////////////////////////////////////////////////////////////////




void AKT_PlayerCharacter::OnChangeWeaponPressed()
{
	ChangeWeaponOnServer();
}


void AKT_PlayerCharacter::DropFirstGrenadeOnServer_Implementation()
{
	if (IsValid(ItemsManagerComponent->FirstGrenadeSlot))
	{
		ItemsManagerComponent->FirstGrenadeSlot->ToUseWeapon(false);
		ItemsManagerComponent->FirstGrenadeSlot->Destroy();
		ItemsManagerComponent->FirstGrenadeSlot = nullptr;
	}
}


void AKT_PlayerCharacter::DropSecondGrenadeOnServer_Implementation()
{
	if (IsValid(ItemsManagerComponent->SecondGrenadeSlot))
	{
		ItemsManagerComponent->SecondGrenadeSlot->ToUseWeapon(false);
		ItemsManagerComponent->SecondGrenadeSlot->Destroy();
		ItemsManagerComponent->SecondGrenadeSlot = nullptr;
	}
}


void AKT_PlayerCharacter::AddGrenade_Implementation(TSubclassOf<AKT_BaseGrenade> InGrenadeClass, const bool InToFirstSlot)
{
	const FAttachmentTransformRules LRules(EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	const FRotator LRotation = GetControlRotation();
	const FActorSpawnParameters LSpawnInfo;
	
	if (InToFirstSlot)
	{
		const FVector LLocation = GetMesh()->GetSocketLocation(ItemsManagerComponent->FirstGrenadeSlotSocketName);
		ItemsManagerComponent->FirstGrenadeSlot = GetWorld()->SpawnActor<AKT_BaseGrenade>(InGrenadeClass, LLocation, LRotation, LSpawnInfo);
		if (IsValid(ItemsManagerComponent->FirstGrenadeSlot))
		{
			ItemsManagerComponent->FirstGrenadeSlot->Initialize(this);
			ItemsManagerComponent->FirstGrenadeSlot->AttachToComponent(GetMesh(), LRules, ItemsManagerComponent->FirstGrenadeSlotSocketName);
		}
		
	}
	else
	{
		const FVector LLocation = GetMesh()->GetSocketLocation(ItemsManagerComponent->SecondGrenadeSlotSocketName);
		ItemsManagerComponent->SecondGrenadeSlot = GetWorld()->SpawnActor<AKT_BaseGrenade>(InGrenadeClass, LLocation, LRotation, LSpawnInfo);

		if (IsValid(ItemsManagerComponent->SecondGrenadeSlot))
		{
			ItemsManagerComponent->SecondGrenadeSlot->Initialize(this);
			ItemsManagerComponent->SecondGrenadeSlot->AttachToComponent(GetMesh(), LRules, ItemsManagerComponent->FirstGrenadeSlotSocketName);
		}
	}
}


void AKT_PlayerCharacter::AddWeapon_Implementation(TSubclassOf<AKT_BaseWeapon> InWeaponClass, const int InAmountOfAmmo, const int AmmoInTheClip)
{
	if (!IsValid(ItemsManagerComponent->FirstWeaponSlot))
	{
		FVector LLocation = FirstPersonMeshComponent->GetSocketLocation(ItemsManagerComponent->InHandsSocketName);
		FRotator LRotation = GetControlRotation();
		FActorSpawnParameters LSpawnInfo;
	
		ItemsManagerComponent->FirstWeaponSlot = GetWorld()->SpawnActor<AKT_BaseWeapon>(InWeaponClass, LLocation, LRotation, LSpawnInfo);
		if (IsValid(ItemsManagerComponent->FirstWeaponSlot))
		{
			ItemsManagerComponent->FirstWeaponSlot->SetOwner(this);
			ItemsManagerComponent->FirstWeaponSlot->Initialize(this, AmmoInTheClip);
			ItemsManagerComponent->FirstWeaponSlot->ToAttachToComponent(FirstPersonMeshComponent, ItemsManagerComponent->InHandsSocketName);
			ItemsManagerComponent->AddAmmo(InWeaponClass, InAmountOfAmmo);
		}
		return;
	}
	if (!IsValid(ItemsManagerComponent->SecondWeaponSlot))
	{
		USkeletalMeshComponent* LMesh = GetMesh();
		const FVector LLocation = LMesh->GetSocketLocation(ItemsManagerComponent->BehindBackSocketName);
		const FRotator LRotation = LMesh->GetSocketRotation(ItemsManagerComponent->BehindBackSocketName);
		const FActorSpawnParameters LSpawnInfo;

		if (IsValid(InWeaponClass))
		{
			ItemsManagerComponent->SecondWeaponSlot = GetWorld()->SpawnActor<AKT_BaseWeapon>(InWeaponClass, LLocation, LRotation, LSpawnInfo);
		}
		if (IsValid(ItemsManagerComponent->SecondWeaponSlot))
		{
			ItemsManagerComponent->SecondWeaponSlot->SetOwner(this);
			ItemsManagerComponent->SecondWeaponSlot->Initialize(this, AmmoInTheClip);
			ItemsManagerComponent->SecondWeaponSlot->ToAttachToComponent(LMesh, ItemsManagerComponent->BehindBackSocketName);
			ItemsManagerComponent->AddAmmo(InWeaponClass, InAmountOfAmmo);
		}
		
		return;
	}
	const FVector LLocation = FirstPersonMeshComponent->GetSocketLocation(ItemsManagerComponent->InHandsSocketName);
	const FRotator LRotation = GetControlRotation();
	const FActorSpawnParameters LSpawnInfo;
	
	ItemsManagerComponent->GetSelectedWeaponSlot()->ToDetachFromActor();
	ItemsManagerComponent->GetSelectedWeaponSlot() = nullptr;

	
	ItemsManagerComponent->GetSelectedWeaponSlot() = GetWorld()->SpawnActor<AKT_BaseWeapon>(InWeaponClass, LLocation, LRotation, LSpawnInfo);
	if (IsValid(ItemsManagerComponent->GetSelectedWeaponSlot()))
	{
		ItemsManagerComponent->GetSelectedWeaponSlot()->SetOwner(this);
		ItemsManagerComponent->GetSelectedWeaponSlot()->Initialize(this, AmmoInTheClip);
		ItemsManagerComponent->GetSelectedWeaponSlot()->ToAttachToComponent(FirstPersonMeshComponent, ItemsManagerComponent->InHandsSocketName);
		ItemsManagerComponent->AddAmmo(InWeaponClass, InAmountOfAmmo);
	}
}


void AKT_PlayerCharacter::ChangeWeaponOnServer_Implementation()
{
	ItemsManagerComponent->ChangeWeapon();
}


void AKT_PlayerCharacter::FireOnServer_Implementation()
{
	if (IsSprinted)
	{
		BreakSprint();
	}
	else
	{
		CanShoot = true;
	}

	
	if (CanShoot)
	{
		ItemsManagerComponent->GetSelectedWeaponSlot()->ToUseWeapon(false);
	}
}


void AKT_PlayerCharacter::StopFireOnServer_Implementation()
{
	CanShoot = false;
}


void AKT_PlayerCharacter::ReloadOnServer_Implementation()
{
	if (IsSprinted)
	{
		BreakSprint();
	}
	Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetSelectedWeaponSlot())->ToReload();
}


void AKT_PlayerCharacter::RightClick()
{
	if (ItemsManagerComponent->GetSelectedWeaponSlot()->CanScope)
	{
		Scope();
		ScopeOnServer();
	}
	else
	{
		AlterFireOnServer();
	}
}

void AKT_PlayerCharacter::RightUnClick_Implementation()
{
	if (ItemsManagerComponent->GetSelectedWeaponSlot()->CanScope)
	{
		UnScope();
		UnScopeOnServer();
	}
	else
	{
		CanShoot = false;
	}
}


void AKT_PlayerCharacter::AlterFireOnServer_Implementation()
{
	if (IsSprinted)
	{
		BreakSprint();
	}
	else
	{
		CanShoot = true;
	}
	
	if (CanShoot)
	{
		ItemsManagerComponent->GetSelectedWeaponSlot()->ToUseWeapon(true);
	}
}


void AKT_PlayerCharacter::ScopingTimeLineFloatReturn(float Value)
{
	
	if (IsScoping)
	{
		FirstPersonMeshComponent->SetRelativeTransform(UKismetMathLibrary::TLerp(FirstPersonMeshComponent->GetRelativeTransform(), CalculateADSTransform(), Value));
	}
	else
	{
		FirstPersonMeshComponent->SetRelativeTransform(UKismetMathLibrary::TLerp(FirstPersonMeshComponent->GetRelativeTransform(), DefaultArmsTransform, Value));
	}
}


FTransform AKT_PlayerCharacter::CalculateADSTransform()
{
	const auto LWeapon = Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetSelectedWeaponSlot());
	return  UKismetMathLibrary::InvertTransform(UKismetMathLibrary::MakeRelativeTransform(LWeapon->Mesh->GetSocketTransform(LWeapon->ScopingSocketName), FirstPersonMeshComponent->GetComponentTransform()));

}


void AKT_PlayerCharacter::Scope_Implementation()
{
	const auto LWeapon = Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetSelectedWeaponSlot());
	if (!LWeapon->IsReloading)
	{
		IsScoping = true;
		ScopingTimeLine->Play();
	}
}


void AKT_PlayerCharacter::ScopeOnServer_Implementation()
{
	Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetSelectedWeaponSlot())->IsScoping = true;
}


void AKT_PlayerCharacter::UnScope_Implementation()
{
	IsScoping = false;
	ScopingTimeLine->Reverse();
}


void AKT_PlayerCharacter::UnScopeOnServer_Implementation()
{
	Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetSelectedWeaponSlot())->IsScoping = false;
}


void AKT_PlayerCharacter::SetCanShootOnServer_Implementation(const bool InCanShoot)
{
	CanShoot = InCanShoot;
}


void AKT_PlayerCharacter::CheckCanFireOnServer_Implementation()
{
	if (!IsSprinted)
	{
		CanShoot = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////Interaction//////////////////////////////////////////////////////////

void AKT_PlayerCharacter::InteractOnServer_Implementation(AKT_BaseInteractiveObject* InInteractiveObject)
{
	InteractiveObject = InInteractiveObject;
	InteractiveObject->ToInteractive(this);
}


void AKT_PlayerCharacter::Interact()
{
	if (IsValid(InteractiveObject) && CanInteract)
	{
		InteractiveObject->ToInteractive(this);
		InteractOnServer(InteractiveObject);
	}
}


void AKT_PlayerCharacter::InteractInfo(AKT_BaseInteractiveObject* InInteractiveObject)
{
	if (IsValid(InInteractiveObject))
	{
		InteractiveObject = InInteractiveObject;
	}
	CanInteract = true;
}


void AKT_PlayerCharacter::UnInteractInfo()
{
	InteractiveObject = nullptr;
	CanInteract = false;
}


void AKT_PlayerCharacter::OnEscapeButtonPressed_Implementation()
{
	if (!HUD->PauseMenu)
	{
		HUD->CreatePauseMenuWD();
	}
	else
	{
		HUD->RemovePauseMenuWD();
	}
}