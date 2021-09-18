#include "Weapons/KT_BaseWeapon.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseWeapon::AKT_BaseWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = Mesh;
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
	
	SetActorEnableCollision(false);
	AttachToComponent(InComponent, LRules, InSocketName);
}


void AKT_BaseWeapon::ToDetachFromActor()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);
}
