#include "Weapons/KT_BaseWeapon.h"

#include "Camera/CameraComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Net/UnrealNetwork.h"


AKT_BaseWeapon::AKT_BaseWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	
	RootComponent = Mesh;

	Mesh->SetCollisionProfileName(FName("IgnoreAll"));
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = false;
}


void AKT_BaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_BaseWeapon, AmmoInTheClip);
	DOREPLIFETIME(AKT_BaseWeapon, Character);
	DOREPLIFETIME(AKT_BaseWeapon, Controller);
	DOREPLIFETIME(AKT_BaseWeapon, UseAlterFire);
	DOREPLIFETIME(AKT_BaseWeapon, IsReloading);
	DOREPLIFETIME(AKT_BaseWeapon, IsShoot);
}


void AKT_BaseWeapon::OnRep_AmmoInTheClip_Implementation()
{
	if (IsValid(Character))
	{
		Character->ItemsManagerComponent->OnAmmoInTheClipChange.Broadcast(AmmoInTheClip);
	}
}


void AKT_BaseWeapon::BreakAllAction_Implementation()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	CanShoot = true;
	IsReloading = false;
	IsChangingFireMode = false;
}


void AKT_BaseWeapon::AutoFireReload()
{
	IsShoot = false;
	CanShoot = true;
	if (!AutoFire) return;
	
	UseWeapon();
}


void AKT_BaseWeapon::ActivateTimerBetweenShots()
{
	if (UseAlterFire)
	{
		GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenAlterShots / Character->BerserkBooster, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(AutoFireTimerHandle, AutoFireTimerDelegate, DelayBetweenShots / Character->BerserkBooster, false);
	}
	CanShoot = false;
}


bool AKT_BaseWeapon::GetWeaponCanShoot() const
{
	if (!CanShoot || !Character->ItemsManagerComponent->WantShoot)
	{
		return false;
	}
	return true;
}


void AKT_BaseWeapon::StartChangeFireMode_Implementation()
{
	if (IsReloading) return;
	
	GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
	
	CanShoot = false;
	IsChangingFireMode = true;
	GetWorldTimerManager().SetTimer(ChangeFireModeTimerHandle, ChangeFireModeTimerDelegate, ChangeFireModeTime / Character->BerserkBooster, false);
}


void AKT_BaseWeapon::EndChangeFireMode_Implementation()
{
	UseAlterFire ^= true;
	CanShoot = true;
	IsChangingFireMode = false;
}


void AKT_BaseWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int32& InAmmoInTheClip)
{
	Character = InCharacter;
	if (IsValid(Character->GetController()))
	{
		Controller = Cast<AKT_PlayerController>(Character->GetController());
	}
	
	AmmoInTheClip = InAmmoInTheClip;
	if (AmmoInTheClip == -1)
	{
		AmmoInTheClip = ClipSize;
	}
	AutoFireTimerDelegate.BindUFunction(this, "AutoFireReload");

	if (CanScope) return;

	ChangeFireModeTimerDelegate.BindUFunction(this, "EndChangeFireMode");
}
