#include "Character/KT_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "Components/KT_HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/KT_CharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/OutputDeviceNull.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/KT_BaseWeapon.h"
#include "InteractiveObjects/KT_BaseInteractiveObject.h"

#include "Components/KT_ItemsManagerComponent.h"
#include "Editor/EditorEngine.h"
#include "GameMode/KT_GameHUD.h"
#include "Net/UnrealNetwork.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"
#include "Weapons/RangeWeapon/KT_WeaponSniperRifle.h"


////////////////////////////////////////////////////Begin Play//////////////////////////////////////////////////////////

AKT_PlayerCharacter::AKT_PlayerCharacter(const FObjectInitializer& ObjInit) :
Super(ObjInit.SetDefaultSubobjectClass<UKT_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetReplicateMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);

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
	
	ScopingTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScopingTimeLine"));
	ScopingInterpFunction.BindUFunction(this, FName("ScopingTimeLineFloatReturn"));

	ItemsManagerComponent->SetIsReplicated(true);
	HealthComponent->SetIsReplicated(true);
}


void AKT_PlayerCharacter::Initialize()
{
	HealthComponent->OnDead.AddDynamic(this, &AKT_PlayerCharacter::Die);

	ParkourCapsuleComponent->OnComponentBeginOverlap.AddDynamic(CharacterMovementComponent, &UKT_CharacterMovementComponent::WallRunningBegin);
	ParkourCapsuleComponent->OnComponentEndOverlap.AddDynamic(CharacterMovementComponent, &UKT_CharacterMovementComponent::WallRunningEnd);

	WallRunRightCollisionComponent->OnComponentBeginOverlap.AddDynamic(CharacterMovementComponent, &UKT_CharacterMovementComponent::WallRunningCameraTiltRight);
	WallRunLeftCollisionComponent->OnComponentBeginOverlap.AddDynamic(CharacterMovementComponent, &UKT_CharacterMovementComponent::WallRunningCameraTiltLeft);

	WallRunRightCollisionComponent->OnComponentEndOverlap.AddDynamic(CharacterMovementComponent, &UKT_CharacterMovementComponent::EndTiltOnWallRunning);
	WallRunLeftCollisionComponent->OnComponentEndOverlap.AddDynamic(CharacterMovementComponent, &UKT_CharacterMovementComponent::EndTiltOnWallRunning);

	if (ScopingCameraTilt)
	{
		ScopingTimeLine->AddInterpFloat(ScopingCameraTilt, ScopingInterpFunction, FName("Alpha"));
		ScopingTimeLine->SetLooping(false);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);

	DefaultArmsTransform = FirstPersonMeshComponent->GetRelativeTransform();

	if (HasAuthority())
	{
		CharacterMovementComponent->CheckAndReloadDash();
	}
}


void AKT_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterMovementComponent = Cast<UKT_CharacterMovementComponent>(GetCharacterMovement());

	check(ItemsManagerComponent);
	check(HealthComponent);
	check(CharacterMovementComponent);

	Initialize();
}


void AKT_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AKT_PlayerCharacter::Interact);

	PlayerInputComponent->BindAction("AlterFire", IE_Pressed, this, &AKT_PlayerCharacter::RightClick);//TODO relocate to ItemsManagerComponent
	PlayerInputComponent->BindAction("AlterFire", IE_Released, this, &AKT_PlayerCharacter::RightUnClick);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, ItemsManagerComponent, &UKT_ItemsManagerComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, ItemsManagerComponent, &UKT_ItemsManagerComponent::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, ItemsManagerComponent, &UKT_ItemsManagerComponent::Reload);

	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, ItemsManagerComponent, &UKT_ItemsManagerComponent::ToChangeWeapon);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, CharacterMovementComponent, &UKT_CharacterMovementComponent::Jumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Dash", IE_Released, CharacterMovementComponent, &UKT_CharacterMovementComponent::Dash);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, CharacterMovementComponent, &UKT_CharacterMovementComponent::ChangeSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, CharacterMovementComponent, &UKT_CharacterMovementComponent::ChangeCrouch);

	PlayerInputComponent->BindAxis("MoveForward", CharacterMovementComponent, &UKT_CharacterMovementComponent::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", CharacterMovementComponent, &UKT_CharacterMovementComponent::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


void AKT_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_PlayerCharacter, CanInteract);
	DOREPLIFETIME(AKT_PlayerCharacter, IsScoping);
	DOREPLIFETIME(AKT_PlayerCharacter, InteractiveObject);
	DOREPLIFETIME(AKT_PlayerCharacter, DamageBooster);
	DOREPLIFETIME(AKT_PlayerCharacter, BerserkBooster);
}


void AKT_PlayerCharacter::SetBerserkBoost_Implementation(const float& Boost)
{
	BerserkBooster = Boost;
}


void AKT_PlayerCharacter::SetRageBoost_Implementation(const float& Boost)
{
	DamageBooster = Boost;
}


void AKT_PlayerCharacter::SetSpeedBoost_Implementation(const float& Boost)
{
	CharacterMovementComponent->SpeedBooster = Boost;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////Weapons//////////////////////////////////////////////////////////////////////


void AKT_PlayerCharacter::RightClick()
{
	if (!IsValid(ItemsManagerComponent->GetSelectedWeaponSlot())) return;

	if (ItemsManagerComponent->GetSelectedWeaponSlot()->CanScope)
	{
		Scope();
		ScopeOnServer();
	}
	else
	{
		ItemsManagerComponent->GetSelectedWeaponSlot()->StartChangeFireMode();
	}
}

void AKT_PlayerCharacter::RightUnClick_Implementation()
{
	if (!IsValid(ItemsManagerComponent->GetSelectedWeaponSlot())) return;
	if (!ItemsManagerComponent->GetSelectedWeaponSlot()->CanScope) return;

	UnScope();
	UnScopeOnServer();
}


void AKT_PlayerCharacter::ScopingTimeLineFloatReturn(float Value)
{
	if (IsScoping)
	{
		FirstPersonMeshComponent->SetRelativeTransform(
			UKismetMathLibrary::TLerp(FirstPersonMeshComponent->GetRelativeTransform(), CalculateADSTransform(),
			                          Value));
	}
	else
	{
		FirstPersonMeshComponent->SetRelativeTransform(
			UKismetMathLibrary::TLerp(FirstPersonMeshComponent->GetRelativeTransform(), DefaultArmsTransform, Value));
	}
}


FTransform AKT_PlayerCharacter::CalculateADSTransform()
{
	const auto LWeapon = Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetFirstPersonSelectedWeaponSlot());
	return UKismetMathLibrary::InvertTransform(UKismetMathLibrary::MakeRelativeTransform(
		LWeapon->Mesh->GetSocketTransform(LWeapon->ScopingSocketName),
		FirstPersonMeshComponent->GetComponentTransform()));
}


void AKT_PlayerCharacter::Scope_Implementation()
{
	const auto LWeapon = Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetFirstPersonSelectedWeaponSlot());
	if (LWeapon->IsReloading) return;
	
	ScopingTimeLine->Play();
	ItemsManagerComponent->GetSelectedWeaponSlot()->Scope();
	IsScoping = true;
}


void AKT_PlayerCharacter::ScopeOnServer_Implementation()
{
	Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetSelectedWeaponSlot())->IsScoping = true;
}


void AKT_PlayerCharacter::UnScope_Implementation()
{
	ScopingTimeLine->Reverse();
	ItemsManagerComponent->GetSelectedWeaponSlot()->UnScope();
	IsScoping = false;
}


void AKT_PlayerCharacter::UnScopeOnServer_Implementation()
{
	Cast<AKT_BaseRangeWeapon>(ItemsManagerComponent->GetSelectedWeaponSlot())->IsScoping = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////Interaction//////////////////////////////////////////////////////////

void AKT_PlayerCharacter::Interact()
{
	if (!IsValid(InteractiveObject) || !CanInteract) return;
	if (!InteractiveObject->CanTake) return;

	CallInteractOnServer();
	InteractiveObject->InteractiveOnClient(this);
}


void AKT_PlayerCharacter::CallInteractOnServer_Implementation()
{
	if (!IsValid(InteractiveObject) || !CanInteract) return;

	InteractiveObject->InteractiveOnServer(this);
}


void AKT_PlayerCharacter::InteractInfoOnServer_Implementation(AKT_BaseInteractiveObject* InInteractiveObject)
{
	InteractiveObject = InInteractiveObject;
	CanInteract = true;
}


void AKT_PlayerCharacter::UnInteractInfo_Implementation()
{
	InteractiveObject = nullptr;
	CanInteract = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////Die//////////////////////////////////////////////////////////////

void AKT_PlayerCharacter::DieOnClient_Implementation()
{
	if (IsValid(GetController()))
	{
		Cast<AKT_PlayerController>(GetController())->RespawnPlayer();
		Cast<AKT_PlayerController>(GetController())->UnPossess();
	}
	if (IsValid(HUD))
	{
		HUD->RemoveMainHUD_WD();
	}
}


void AKT_PlayerCharacter::Die(AController* Player)
{
	if (IsValid(FirstPersonMeshComponent))
	{
		FirstPersonMeshComponent->DestroyComponent();
	}

	const FVector LLocation = GetActorLocation() + FVector(0, 0, -60);
	const FRotator LRotation = GetActorRotation();
	const FActorSpawnParameters LSpawnInfo;


	if (HasAuthority())
	{
		FTimerHandle LDestroyTimerHandle;
		FTimerDelegate LDestroyTimerDelegate;

		DieOnClient();
		DieMulticast();

		LDestroyTimerDelegate.BindUFunction(this, "Destruction");
		GetWorldTimerManager().SetTimer(LDestroyTimerHandle, LDestroyTimerDelegate, 5, false);
	}
	OnDead.Broadcast(this);
	PlayerController = nullptr;
}


void AKT_PlayerCharacter::DieMulticast_Implementation()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (IsValid(CapsuleComp))
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		CapsuleComp->SetCollisionProfileName("IgnoreAll");
		CapsuleComp->SetGenerateOverlapEvents(false);
		CapsuleComp->SetNotifyRigidBodyCollision(false);
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionProfileName("PhysicsActor");
	GetMesh()->SetSimulatePhysics(true);


	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}
}


void AKT_PlayerCharacter::Destruction()
{
	// if (IsValid(ItemsManagerComponent->FirstWeaponSlot))
	// {
	// 	ItemsManagerComponent->FirstWeaponSlot->Destroy();
	// }
	// else if (IsValid(ItemsManagerComponent->SecondWeaponSlot))
	// {
	// 	ItemsManagerComponent->SecondWeaponSlot->Destroy();
	// }

	Destroy();
}
