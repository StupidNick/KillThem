#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"

#include "DrawDebugHelpers.h"
#include "Character/KT_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


void AKT_BaseRangeWeapon::UseWeapon()
{
	Super::UseWeapon();

	if (ProjectileShooting)
	{
		ProjectileShoot();
	}
	else
	{
		LineTraceProjectile();
	}
}


void AKT_BaseRangeWeapon::ProjectileShoot()
{
	const FVector LLocation = Mesh->GetSocketTransform(FireSocketName).GetLocation();
	const FRotator LRotation = Mesh->GetSocketTransform(FireSocketName).GetRotation().Rotator();
	const FActorSpawnParameters LSpawnInfo;

	AKT_BaseProjectile* LProjectile = GetWorld()->SpawnActor<AKT_BaseProjectile>(ProjectileClass, LLocation, LRotation, LSpawnInfo);
	LProjectile->Initialize(Damage, Character, this);
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
		DrawDebugBox(GetWorld(), LHitResult.ImpactPoint, FVector(10,10,10), FColor::Emerald, false, 2);
	}
	if (LHitResult.Actor != Character && LbHit)
	{
		const FDamageEvent LDamageEvent;
		const TSubclassOf<UDamageType> LDamageTypeClass;
		
		UGameplayStatics::ApplyDamage(LHitResult.GetActor(), Damage, LHitResult.GetActor()->GetInstigatorController(), Character, LDamageTypeClass);
	}
}
