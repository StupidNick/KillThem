#include "Weapons/KT_BaseWeapon.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Net/UnrealNetwork.h"


AKT_BaseWeapon::AKT_BaseWeapon()
{
	// BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	
	RootComponent = Mesh;
	// BoxCollision->SetupAttachment(Mesh);

	Mesh->SetCollisionProfileName(FName("IgnoreAll"));
	// BoxCollision->SetCollisionProfileName(FName("IgnoreAll"));
	bReplicates = true;
}


void AKT_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_BaseWeapon::UseWeapon()
{
}


void AKT_BaseWeapon::AutoFireReload()
{
	CanShoot = true;
	
	if (HasAuthority() && Character->CanShoot)
	{
		if (AutoFire)
		{
			UseWeapon();
			
			CanShoot = false;
			GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenShots / Character->BerserkBooster, false);
		}
	}
	else
	{
		Character->CheckCanFireOnServer();
	}
}


void AKT_BaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_BaseWeapon, AmmoInTheClip);
}


void AKT_BaseWeapon::OnRep_AmmoInTheClip_Implementation()
{
	if (IsValid(GetOwner()) && !HasAuthority())
	{
		Cast<AKT_PlayerCharacter>(GetOwner())->ItemsManagerComponent->OnAmmoInTheClipChange.Broadcast(AmmoInTheClip);
	}
}


void AKT_BaseWeapon::ToUseWeapon(const bool IsAlterFire)
{
	UseAlterFire = IsAlterFire;
	
	if (CanShoot)
	{
		if (UseAlterFire)
		{
			AlterFireTimerDelegate.BindUFunction(this, "UseWeapon");
			GetWorldTimerManager().SetTimer(AlterFireHandle, AlterFireTimerDelegate, TimeBeforeAlterFire / Character->BerserkBooster, false);
		}
		else
		{
			UseWeapon();
		}
		
		if (DelayBetweenShots > 0)
		{
			CanShoot = false;
			
			GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenShots / Character->BerserkBooster, false);
		}
	}
}


void AKT_BaseWeapon::StopFire()
{
}


void AKT_BaseWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int InAmmoInTheClip)
{
	if (HasAuthority())
	{
		AmmoInTheClip = InAmmoInTheClip;
		if (AmmoInTheClip == -1)
		{
			AmmoInTheClip = ClipSize;
		}
	}
	Character = InCharacter;
	AutoFireTimerDelegate.BindUFunction(this, "AutoFireReload");
}


void AKT_BaseWeapon::ToAttachToComponent_Implementation(USkeletalMeshComponent* InComponent, const FName InSocketName)
{
	const FAttachmentTransformRules LRules(EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	
	Mesh->AttachToComponent(InComponent, LRules, InSocketName);
}


void AKT_BaseWeapon::Detach_Implementation()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}


void AKT_BaseWeapon::ToDetachFromActor_Implementation()
{
	const FVector LLocation = GetActorLocation() + FVector(100, 0, 0);
	const FRotator LRotation = GetActorRotation();
	const FActorSpawnParameters LSpawnInfo;

	if (HasAuthority())
	{
		AKT_BaseInteractiveWeapon* LDroppedWeapon = GetWorld()->SpawnActor<AKT_BaseInteractiveWeapon>(InteractiveWeaponClass, LLocation, LRotation, LSpawnInfo);
		if (IsValid(LDroppedWeapon))
		{
			LDroppedWeapon->Initialize(AmmoInTheClip);
			LDroppedWeapon->SkeletalMesh->SetCollisionProfileName(FName("BlockAll"));
			LDroppedWeapon->SkeletalMesh->SetSimulatePhysics(true);
		}
	}
	Destroy();
}
