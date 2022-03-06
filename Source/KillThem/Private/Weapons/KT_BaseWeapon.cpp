#include "Weapons/KT_BaseWeapon.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Net/UnrealNetwork.h"


AKT_BaseWeapon::AKT_BaseWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	
	RootComponent = Mesh;

	Mesh->SetCollisionProfileName(FName("IgnoreAll"));
	bReplicates = true;
}


void AKT_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_BaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_BaseWeapon, AmmoInTheClip);
}


void AKT_BaseWeapon::OnRep_AmmoInTheClip_Implementation()
{
	if (IsValid(Character))
	{
		Character->ItemsManagerComponent->OnAmmoInTheClipChange.Broadcast(AmmoInTheClip);
	}
}


void AKT_BaseWeapon::UseWeapon()
{
	CanShoot = false;
	if (UseAlterFire)
	{
		GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenAlterShots / Character->BerserkBooster, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenShots / Character->BerserkBooster, false);
	}
}


void AKT_BaseWeapon::AutoFireReload()
{
	CanShoot = true;
	
	if (HasAuthority() && Character->ItemsManagerComponent->CanShoot)
	{
		if (AutoFire)
		{
			UseWeapon();
			
			CanShoot = false;
			if (UseAlterFire)
			{
				GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenAlterShots / Character->BerserkBooster, false);
			}
			else
			{
				GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenShots / Character->BerserkBooster, false);
			}
		}
	}
	else if (HasAuthority())
	{
		Character->CheckCanFireOnServer();
	}
}


void AKT_BaseWeapon::ToUseWeapon(const bool IsAlterFire)
{
	UseAlterFire = IsAlterFire;
	
	if (CanShoot && !Character->IsSprinted)
	{
		if (UseAlterFire)
		{
			CanShoot = false;
			GetWorldTimerManager().SetTimer(AlterFireHandle, AlterFireTimerDelegate, TimeBeforeAlterFire / Character->BerserkBooster, false);
		}
		else
		{
			UseWeapon();
		}
	}
}


void AKT_BaseWeapon::StopFire()
{
}


void AKT_BaseWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int InAmmoInTheClip)
{
	Character = InCharacter;
	AmmoInTheClip = InAmmoInTheClip;
	if (AmmoInTheClip == -1)
	{
		AmmoInTheClip = ClipSize;
	}
	AlterFireTimerDelegate.BindUFunction(this, "UseWeapon");
	AutoFireTimerDelegate.BindUFunction(this, "AutoFireReload");
}
