#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
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

	if (AmmoInTheClip <= 0 || IsReloading) return;

	if (UseAlterFire)
	{
		if (ProjectileShootingAtAlterFire)
		{
			ProjectileShoot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, AlterFireScatterFactor,
			                CostAlterShotInAmmo);
		}
		else
		{
			LineTraceShot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, AlterFireScatterFactor,
			              CostAlterShotInAmmo);
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


void AKT_BaseRangeWeapon::ProjectileShoot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass,
                                          const int32& InDamage,
                                          const FName& InShotSocketName, const float& InScatterFactor,
                                          const int32& InSpentAmmo)
{
	if (!IsValid(Character)) return;
	if (!IsValid(Controller)) return;

	FVector LStartLocation, LEndLocation;
	if (!GetTraceData(LStartLocation, LEndLocation, InScatterFactor)) return;

	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(255, 0, 0), false, 5, 0, 1.0);

	FHitResult LHitResult;
	MakeHit(LHitResult, LStartLocation, LEndLocation);

	SpawnProjectile(LHitResult, LEndLocation, InShotSocketName, InProjectileClass, InDamage);
	
	AmmoInTheClip -= InSpentAmmo;
}


void AKT_BaseRangeWeapon::LineTraceShot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage,
                                        const FName& InShotSocketName, const float& InScatterFactor,
                                        const int32& InSpentAmmo)
{
	if (!IsValid(Character)) return;
	if (!IsValid(Controller)) return;

	FVector LStartLocation, LEndLocation;
	if (!GetTraceData(LStartLocation, LEndLocation, InScatterFactor)) return;

	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(0, 0, 255), false, 5, 0, 5.0); //TODO debug

	FHitResult LHitResult;
	MakeHit(LHitResult, LStartLocation, LEndLocation);

	AmmoInTheClip -= InSpentAmmo;
	if (!LHitResult.Actor.IsValid()) return;

	SpawnProjectile(LHitResult, LEndLocation, InShotSocketName, InProjectileClass);

	if (IsValid(DamageTypeClass) && LHitResult.Actor->GetClass() == Character->GetClass())
	{
		UGameplayStatics::ApplyDamage(LHitResult.GetActor(), InDamage * Character->DamageBooster,
		                              LHitResult.GetActor()->GetInstigatorController(), Character, DamageTypeClass);
	}
}


bool AKT_BaseRangeWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	if (!IsValid(Controller)) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}


FVector AKT_BaseRangeWeapon::GetMuzzleWorldLocation(const FName& SocketName) const
{
	return Mesh->GetSocketLocation(SocketName);
}


bool AKT_BaseRangeWeapon::GetTraceData(FVector& StartLocation, FVector& EndLocation, const float InScatterFactor) const
{
	FVector LViewLocation;
	FRotator LViewRotation;
	if (!GetPlayerViewPoint(LViewLocation, LViewRotation)) return false;

	StartLocation = LViewLocation;
	const FVector LShootDirection = LViewRotation.Vector() + FVector(FMath::FRandRange(-0.05, 0.05) * InScatterFactor,
	                                                                 FMath::FRandRange(-0.05, 0.05) * InScatterFactor,
	                                                                 FMath::FRandRange(-0.05, 0.05) * InScatterFactor);
	EndLocation = StartLocation + MaxDistanceAttack * LShootDirection;
	return true;
}


void AKT_BaseRangeWeapon::MakeHit(FHitResult& HitResult, const FVector& StartLocation, const FVector& EndLocation) const
{
	FCollisionQueryParams LCollisionParams;

	LCollisionParams.AddIgnoredActor(this);
	LCollisionParams.AddIgnoredActor(Character);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility,
	                                     LCollisionParams);
}


void AKT_BaseRangeWeapon::SpawnProjectile(const FHitResult& HitResult, const FVector& EndLocation, const FName& SocketName,
	const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage)
{
	const FTransform LSpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation(SocketName));
	AKT_BaseProjectile* LProjectile = GetWorld()->SpawnActorDeferred<AKT_BaseProjectile>(
		InProjectileClass, LSpawnTransform);
	if (IsValid(LProjectile))
	{
		const FVector LEndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : EndLocation;
		const FVector LDirection = (LEndPoint - GetMuzzleWorldLocation(SocketName)).GetSafeNormal();
		LProjectile->SetShootDirection(LDirection);
		LProjectile->Initialize(InDamage * Character->DamageBooster, Character, this);
		LProjectile->FinishSpawning(LSpawnTransform);
	}
}


void AKT_BaseRangeWeapon::Reload(const int InAmmo)
{
	IsReloading = false;
	AmmoInTheClip += InAmmo;
	Character->ItemsManagerComponent->RemoveAmmoOnServer(GetClass(), InAmmo);
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
	if (Character->ItemsManagerComponent->CountAmmo(GetClass(), LCountOfAmmo) && AmmoInTheClip < LClipSize)
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
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadTimerDelegate, ReloadTime / Character->BerserkBooster,
		                                false);
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
