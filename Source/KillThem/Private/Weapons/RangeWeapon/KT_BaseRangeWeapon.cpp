#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"

#include "DrawDebugHelpers.h"
#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


AKT_BaseRangeWeapon::AKT_BaseRangeWeapon()
{
	
}

void AKT_BaseRangeWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter)
{
	Super::Initialize_Implementation(InCharacter);
	
	AmmoInTheClip = ClipSize;
}



void AKT_BaseRangeWeapon::UseWeapon()
{
	if (AmmoInTheClip > 0 && !IsReloading)
	{
		if (ProjectileShooting)
		{
			ProjectileShoot();
		}
		else
		{
			LineTraceProjectile();
		}
	}
	
	Super::UseWeapon();
}


void AKT_BaseRangeWeapon::ProjectileShoot()
{
	const FVector LLocation = Mesh->GetSocketTransform(FireSocketName).GetLocation();
	const FRotator LRotation = Character->Controller->GetControlRotation();
	FActorSpawnParameters LSpawnInfo;

	LSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	LSpawnInfo.Owner = this;
	LSpawnInfo.Instigator = Character;

	AKT_BaseProjectile* LProjectile = GetWorld()->SpawnActor<AKT_BaseProjectile>(ProjectileClass, LLocation, LRotation, LSpawnInfo);
	LProjectile->Initialize(Damage, Character, this);
	AmmoInTheClip--;
	
	UE_LOG(LogTemp, Error, TEXT("%i"), AmmoInTheClip);
}


void AKT_BaseRangeWeapon::LineTraceProjectile()
{
	FCollisionQueryParams LParams;
	
	FHitResult LHitResult;
	FVector LStartLocation;
	FRotator LStartRotation;
	FVector LEndLocation;
	Character->Controller->GetPlayerViewPoint(LStartLocation, LStartRotation);
	LEndLocation = LStartLocation + MaxDistanceAttack * LStartRotation.Vector();
	
	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(255, 0, 0), false, 5, 0, 5.0);
	LParams.AddIgnoredActor(this);
	LParams.AddIgnoredActor(Character);
	
	bool LbHit = GetWorld()->LineTraceSingleByChannel(LHitResult, LStartLocation, LEndLocation, ECollisionChannel::ECC_GameTraceChannel1, LParams);
	if (LbHit)
	{
		DrawDebugBox(GetWorld(), LHitResult.ImpactPoint, FVector(5,5,5), FColor::Emerald, false, 2);
	}
	if (LHitResult.Actor != Character && LbHit && IsValid(DamageTypeClass))
	{
		const FDamageEvent LDamageEvent;
		
		UGameplayStatics::ApplyDamage(LHitResult.GetActor(), Damage, LHitResult.GetActor()->GetInstigatorController(), Character, DamageTypeClass);
	}
	AmmoInTheClip--;
	
	UE_LOG(LogTemp, Error, TEXT("%i"), AmmoInTheClip);
}

void AKT_BaseRangeWeapon::Reload(const int InAmmo)
{
	IsReloading = false;
	AmmoInTheClip += InAmmo;
}


void AKT_BaseRangeWeapon::ToReload()
{
	int LCountOfAmmo;
	if (AmmoInTheClip < ClipSize ++ && Character->ItemsManagerComponent->FindAndCountAmmo(GetClass(), LCountOfAmmo))
	{
		IsReloading = true;

		if (LCountOfAmmo > ClipSize - AmmoInTheClip)
		{
			LCountOfAmmo = ClipSize - AmmoInTheClip;
			Character->ItemsManagerComponent->RemoveAmmo(GetClass(), LCountOfAmmo);
		}
		else
		{
			Character->ItemsManagerComponent->RemoveAmmo(GetClass(), LCountOfAmmo);
		}
		ReloadTimerDelegate.BindUFunction(this, "Reload", LCountOfAmmo);
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadTimerDelegate, ReloadTime, false);
	}
}
