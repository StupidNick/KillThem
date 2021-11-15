#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AKT_BaseRangeWeapon::AKT_BaseRangeWeapon()
{
	
}

void AKT_BaseRangeWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int InAmmoInTheClip)
{
	Super::Initialize_Implementation(InCharacter, InAmmoInTheClip);
	
	ScatterFactor = BaseScatterFactor;
}



void AKT_BaseRangeWeapon::UseWeapon()
{
	Super::UseWeapon();
	
	if (AmmoInTheClip > 0 && !IsReloading)
	{
		if (UseAlterFire)
		{
			if (ProjectileShootingAtAlterFire)
			{
				ProjectileShoot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, CostAlterShotInAmmo);
			}
			else
			{
				LineTraceShot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, CostAlterShotInAmmo);
			}
		}
		else
		{
			if (ProjectileShooting)
			{
				ProjectileShoot(ProjectileClass, Damage, FireSocketName);
			}
			else
			{
				LineTraceShot(ProjectileClass, Damage, FireSocketName);
			}
		}
	}
}


void AKT_BaseRangeWeapon::ProjectileShoot(const TSubclassOf<AKT_BaseProjectile> InProjectileClass, const int InDamage, const FName InShotSocketName, const int InSpentAmmo)
{
	FCollisionQueryParams LParams;
	
	FHitResult LHitResult;
	FVector LStartLocation;
	FVector LEndLocation;
	FRotator LStartRotation;
	LStartLocation = Character->CameraComponent->GetSocketLocation(FName(""));
	LEndLocation = LStartLocation + MaxDistanceAttack * Character->CameraComponent->GetForwardVector();
	
	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(255, 0, 0), false, 5, 0, 5.0);
	LParams.AddIgnoredActor(this);
	LParams.AddIgnoredActor(Character);
	
	bool LbHit = GetWorld()->LineTraceSingleByChannel(LHitResult, LStartLocation, LEndLocation, ECollisionChannel::ECC_GameTraceChannel1, LParams);
	if (LbHit)
	{
		LStartRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketTransform(FireSocketName).GetLocation(), LHitResult.Location);
	}
	else
	{
		LStartRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketTransform(FireSocketName).GetLocation(), LEndLocation);
		
	}
	LStartRotation += FRotator(FMath::FRandRange(-5, 5) * ScatterFactor, FMath::FRandRange(-5, 5) * ScatterFactor, FMath::FRandRange(-5, 5) * ScatterFactor);

	FActorSpawnParameters LSpawnInfo;
	LSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	LSpawnInfo.Owner = this;
	LSpawnInfo.Instigator = Character;

	AKT_BaseProjectile* LProjectile = GetWorld()->SpawnActor<AKT_BaseProjectile>(InProjectileClass, Mesh->GetSocketTransform(InShotSocketName).GetLocation(), LStartRotation, LSpawnInfo);
	if (IsValid(LProjectile))
	{
		LProjectile->Initialize(InDamage * Character->DamageBooster, Character, this);
		AmmoInTheClip -= InSpentAmmo;
	
		UE_LOG(LogTemp, Error, TEXT("%i"), AmmoInTheClip);
	}
	DrawDebugBox(GetWorld(), LHitResult.ImpactPoint, FVector(5,5,5), FColor::Emerald, false, 2);
}


void AKT_BaseRangeWeapon::LineTraceShot(const TSubclassOf<AKT_BaseProjectile> InProjectileClass, const int InDamage, const FName InShotSocketName, const int InSpentAmmo)
{
	FCollisionQueryParams LParams;
	
	FHitResult LHitResult;
	FVector LStartLocation;
	FVector LEndLocation;
	FRotator LStartRotation;
	LStartLocation = Character->CameraComponent->GetSocketLocation(FName(""));
	LEndLocation = LStartLocation + MaxDistanceAttack * (Character->CameraComponent->GetForwardVector() + FVector(FMath::FRandRange(-0.05, 0.05) * ScatterFactor, FMath::FRandRange(-0.05, 0.05) * ScatterFactor, FMath::FRandRange(-0.05, 0.05) * ScatterFactor));
	
	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(255, 0, 0), false, 5, 0, 5.0);
	LParams.AddIgnoredActor(this);
	LParams.AddIgnoredActor(Character);
	
	bool LbHit = GetWorld()->LineTraceSingleByChannel(LHitResult, LStartLocation, LEndLocation, ECollisionChannel::ECC_GameTraceChannel1, LParams);
	if (LbHit)
	{
		LStartRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketTransform(FireSocketName).GetLocation(), LHitResult.Location);
	}
	else
	{
		LStartRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketTransform(FireSocketName).GetLocation(), LEndLocation);
	}
	
	LStartRotation += FRotator(FMath::FRandRange(-5, 5) * ScatterFactor, FMath::FRandRange(-5, 5) * ScatterFactor, FMath::FRandRange(-5, 5) * ScatterFactor);

	FActorSpawnParameters LSpawnInfo;
	LSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	LSpawnInfo.Owner = this;
	LSpawnInfo.Instigator = Character;

	GetWorld()->SpawnActor<AKT_BaseProjectile>(InProjectileClass, Mesh->GetSocketTransform(InShotSocketName).GetLocation(), LStartRotation, LSpawnInfo);
	DrawDebugBox(GetWorld(), LHitResult.ImpactPoint, FVector(5,5,5), FColor::Emerald, false, 2);
	if (LHitResult.Actor != Character && LbHit && IsValid(DamageTypeClass))
	{
		const FDamageEvent LDamageEvent;

		UGameplayStatics::ApplyDamage(LHitResult.GetActor(), InDamage * Character->DamageBooster, LHitResult.GetActor()->GetInstigatorController(), Character, DamageTypeClass);
	}
	AmmoInTheClip -= InSpentAmmo;
	
	UE_LOG(LogTemp, Error, TEXT("%i"), AmmoInTheClip);
}


void AKT_BaseRangeWeapon::Reload(const int InAmmo)
{
	IsReloading = false;
	AmmoInTheClip += InAmmo;
	Character->ItemsManagerComponent->RemoveAmmo(GetClass(), InAmmo);
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
		}
		ReloadTimerDelegate.BindUFunction(this, "Reload", LCountOfAmmo);
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadTimerDelegate, ReloadTime / Character->BerserkBooster, false);
	}
}


void AKT_BaseRangeWeapon::SetScatterFactor(const float InScatterFactor)
{
	ScatterFactor = InScatterFactor;
}