#include "Weapons/KT_BaseWeapon.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseWeapon::AKT_BaseWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
}


void AKT_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AKT_BaseWeapon::UseWeapon()
{
}


void AKT_BaseWeapon::UseWeaponOnServer_Implementation()
{
	UseWeapon();
}


void AKT_BaseWeapon::ToUseWeapon()
{
	UseWeapon();
	// UseWeaponOnServer();
}


void AKT_BaseWeapon::Initialize(AKT_PlayerCharacter* InCharacter)
{
	Character = InCharacter;

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
