#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


AKT_BaseRangeWeapon::AKT_BaseRangeWeapon()
{
}


void AKT_BaseRangeWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int InAmmoInTheClip)
{
	Super::Initialize_Implementation(InCharacter, InAmmoInTheClip);
	
	ScatterFactor = BaseScatterFactor;
}


void AKT_BaseRangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_BaseRangeWeapon, IsScoping);
	DOREPLIFETIME(AKT_BaseRangeWeapon, Character);
	DOREPLIFETIME(AKT_BaseRangeWeapon, IsReloading);
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
				ProjectileShoot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, AlterFireScatterFactor, CostAlterShotInAmmo);
			}
			else
			{
				LineTraceShot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, AlterFireScatterFactor, CostAlterShotInAmmo);
			}
		}
		else
		{
			if (ProjectileShooting)
			{
				ProjectileShoot(ProjectileClass, Damage, FireSocketName, ScatterFactor);
			}
			else
			{
				LineTraceShot(ProjectileClass, Damage, FireSocketName, ScatterFactor);
			}
		}
	}
}


void AKT_BaseRangeWeapon::ProjectileShoot(const TSubclassOf<AKT_BaseProjectile> InProjectileClass, const int InDamage, const FName InShotSocketName, const float InScatterFactor, const int InSpentAmmo)
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
	LStartRotation += FRotator(FMath::FRandRange(-5, 5) * InScatterFactor, FMath::FRandRange(-5, 5) * InScatterFactor, FMath::FRandRange(-5, 5) * InScatterFactor);

	FActorSpawnParameters LSpawnInfo;
	LSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	LSpawnInfo.Owner = this;
	LSpawnInfo.Instigator = Character;

	AKT_BaseProjectile* LProjectile = GetWorld()->SpawnActor<AKT_BaseProjectile>(InProjectileClass, Mesh->GetSocketTransform(InShotSocketName).GetLocation(), LStartRotation, LSpawnInfo);
	if (IsValid(LProjectile))
	{
		LProjectile->Initialize(InDamage * Character->DamageBooster, Character, this);
		AmmoInTheClip -= InSpentAmmo;
	}
	DrawDebugBox(GetWorld(), LHitResult.ImpactPoint, FVector(5,5,5), FColor::Emerald, false, 2);
}


void AKT_BaseRangeWeapon::LineTraceShot(const TSubclassOf<AKT_BaseProjectile> InProjectileClass, const int InDamage, const FName InShotSocketName, const float InScatterFactor, const int InSpentAmmo)
{
	FCollisionQueryParams LParams;
	
	FHitResult LHitResult;
	FVector LStartLocation;
	FVector LEndLocation;
	FRotator LStartRotation;
	LStartLocation = Character->CameraComponent->GetSocketLocation(FName(""));
	LEndLocation = LStartLocation + MaxDistanceAttack * (Character->CameraComponent->GetForwardVector() + FVector(FMath::FRandRange(-0.05, 0.05) * InScatterFactor, FMath::FRandRange(-0.05, 0.05) * InScatterFactor, FMath::FRandRange(-0.05, 0.05) * InScatterFactor));
	
	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(255, 0, 0), false, 5, 0, 5.0);
	LParams.AddIgnoredActor(this);
	LParams.AddIgnoredActor(Character);
	
	bool LbHit = GetWorld()->LineTraceSingleByChannel(LHitResult, LStartLocation, LEndLocation, ECollisionChannel::ECC_Visibility, LParams);
	if (LbHit)
	{
		LStartRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketTransform(FireSocketName).GetLocation(), LHitResult.Location);
	}
	else
	{
		LStartRotation = UKismetMathLibrary::FindLookAtRotation(Mesh->GetSocketTransform(FireSocketName).GetLocation(), LEndLocation);
	}
	
	LStartRotation += FRotator(FMath::FRandRange(-5, 5) * InScatterFactor, FMath::FRandRange(-5, 5) * InScatterFactor, FMath::FRandRange(-5, 5) * InScatterFactor);

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
}


void AKT_BaseRangeWeapon::Reload(const int InAmmo)
{
	IsReloading = false;
	AmmoInTheClip += InAmmo;
	Character->ItemsManagerComponent->RemoveAmmo(GetClass(), InAmmo);
}


void AKT_BaseRangeWeapon::ToReload_Implementation()
{
	int LCountOfAmmo;
	int LClipSize = ClipSize;
	LClipSize++;
	if (Character->IsScoping)
	{
		Character->UnScope_Implementation();
	}
	if (Character->ItemsManagerComponent->FindAndCountAmmo(GetClass(), LCountOfAmmo) && AmmoInTheClip < LClipSize)
	{
		IsReloading = true;
		if (AmmoInTheClip > 0)
		{
			if (LCountOfAmmo >= LClipSize - AmmoInTheClip)
			{
				LCountOfAmmo = LClipSize - AmmoInTheClip;
			}
		}
		else
		{
			if (LCountOfAmmo >= LClipSize - 1 - AmmoInTheClip)
			{
				LCountOfAmmo = LClipSize - 1 - AmmoInTheClip;
			}
		}
		ReloadTimerDelegate.BindUFunction(this, "Reload", LCountOfAmmo);
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadTimerDelegate, ReloadTime / Character->BerserkBooster, false);
	}
	
}


void AKT_BaseRangeWeapon::OnRep_Scoping_Implementation()
{
	if (IsScoping)
	{
		SetScatterFactor(ScopeScatterFactor);
	}
	else
	{
		SetScatterFactor(BaseScatterFactor);
	}
}


void AKT_BaseRangeWeapon::SetScatterFactor(const float InScatterFactor)
{
	ScatterFactor = InScatterFactor;
}
