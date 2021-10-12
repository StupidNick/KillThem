#include "Weapons/KT_BaseWeapon.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseWeapon::AKT_BaseWeapon()
{
	// BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	
	RootComponent = Mesh;
	// BoxCollision->SetupAttachment(Mesh);

	Mesh->SetCollisionProfileName(FName("IgnoreAll"));
	// BoxCollision->SetCollisionProfileName(FName("IgnoreAll"));
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
	if (Character->CanShoot)
	{
		if (AutoFire)
		{
			UseWeapon();
			CanShoot = false;
			GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenShots, false);
		}
	}
}


void AKT_BaseWeapon::ToUseWeapon()
{
	if (CanShoot)
	{
		UseWeapon();
		
		if (DelayBetweenShots > 0)
		{
			CanShoot = false;
			
			GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenShots, false);
		}
	}
}


void AKT_BaseWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter)
{
	Character = InCharacter;
	AutoFireTimerDelegate.BindUFunction(this, "AutoFireReload");
}


void AKT_BaseWeapon::ToAttachToComponent(USkeletalMeshComponent*& InComponent, const FName InSocketName)
{
	const FAttachmentTransformRules LRules(EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	
	Mesh->AttachToComponent(InComponent, LRules, InSocketName);
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


void AKT_BaseWeapon::ToDetachFromActorOnServer_Implementation()
{
}
