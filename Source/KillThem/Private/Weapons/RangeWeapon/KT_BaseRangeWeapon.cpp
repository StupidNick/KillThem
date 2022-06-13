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


void AKT_BaseRangeWeapon::Initialize_Implementation(AKT_PlayerCharacter* InCharacter, const int32& InAmmoInTheClip)
{
	Super::Initialize_Implementation(InCharacter, InAmmoInTheClip);

	ScatterFactor = BaseScatterFactor;
	if (!ClipReload)
	{
		ReloadTimerDelegate.BindUFunction(this, "Reload", 1);
	}
}


void AKT_BaseRangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_BaseRangeWeapon, IsScoping);
	DOREPLIFETIME(AKT_BaseRangeWeapon, Character);
	DOREPLIFETIME(AKT_BaseRangeWeapon, IsOneBulletReloading);
	DOREPLIFETIME(AKT_BaseRangeWeapon, IsSendingBullet);
	DOREPLIFETIME(AKT_BaseRangeWeapon, IsEndingReload);
}


void AKT_BaseRangeWeapon::UseWeapon()
{
	if (AmmoInTheClip <= 0 || !GetWeaponCanShoot()) return;
	if (IsReloading && !ClipReload)
	{
		if (IsEndingReload) return;
		
		EndBulletReloading();
		return;
	}
	if(IsReloading)
	{
		return;
	}

	IsShoot = true;
	if (UseAlterFire)
	{
		if (AmmoInTheClip < CostAlterShotInAmmo) return;

		if (ProjectileShootingAtAlterFire)
		{
			ProjectileShoot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, AlterFireScatterFactor, AlterFireParticle);
		}
		else
		{
			LineTraceShot(AlterFireProjectileClass, AlterDamage, AlterFireSocketName, AlterFireScatterFactor, AlterFireParticle);
		}
		AmmoInTheClip -= CostAlterShotInAmmo;
	}
	else
	{
		if (ProjectileShooting)
		{
			ProjectileShoot(ProjectileClass, Damage, FireSocketName, ScatterFactor, FireParticle);
		}
		else
		{
			LineTraceShot(ProjectileClass, Damage, FireSocketName, ScatterFactor, FireParticle);
		}
		AmmoInTheClip--;
	}
	ActivateTimerBetweenShots();
	PlayAnimation(ShootAnimation);
}


void AKT_BaseRangeWeapon::ProjectileShoot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass,
                                          const int32& InDamage,
                                          const FName& InShotSocketName, const float& InScatterFactor, UParticleSystem* MuzzleParticle)
{
	if (!IsValid(Character)) return;

	FVector LStartLocation, LEndLocation;
	if (!GetTraceData(LStartLocation, LEndLocation, InScatterFactor)) return;

	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(255, 0, 0), false, 5, 0, 1.0);

	FHitResult LHitResult;
	MakeHit(LHitResult, LStartLocation, LEndLocation);

	SpawnMuzzleFlash(MuzzleParticle, InShotSocketName);
	SpawnProjectile(LHitResult, LEndLocation, InShotSocketName, InProjectileClass, InDamage);
}


void AKT_BaseRangeWeapon::LineTraceShot(const TSubclassOf<AKT_BaseProjectile>& InProjectileClass, const int32& InDamage,
                                        const FName& InShotSocketName, const float& InScatterFactor, UParticleSystem* MuzzleParticle)
{
	if (!IsValid(Character)) return;
	
	FVector LStartLocation, LEndLocation;
	if (!GetTraceData(LStartLocation, LEndLocation, InScatterFactor)) return;

	DrawDebugLine(GetWorld(), LStartLocation, LEndLocation, FColor(0, 0, 255), false, 5, 0, 5.0); //TODO debug

	FHitResult LHitResult;
	MakeHit(LHitResult, LStartLocation, LEndLocation);
	if (LHitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), LHitResult.ImpactPoint, 5, 5, FColor::Red, false, 5, 0, 5.0); //TODO debug
	}
	
	SpawnMuzzleFlash(MuzzleParticle, InShotSocketName);
	SpawnProjectile(LHitResult, LEndLocation, InShotSocketName, InProjectileClass);
	
	if (!LHitResult.Actor.IsValid()) return;

	const TSubclassOf<UDamageType> LDamageType = GetDamageType(LHitResult);
	if (IsValid(LDamageType) && LHitResult.Actor->GetClass() == Character->GetClass())
	{
		UGameplayStatics::ApplyDamage(LHitResult.GetActor(), InDamage * Character->DamageBooster,
		                              Character->Controller, Character, LDamageType);
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
	const auto LHalfRad = FMath::DegreesToRadians(InScatterFactor);
	const FVector LShootDirection = FMath::VRandCone(LViewRotation.Vector(), LHalfRad);
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


void AKT_BaseRangeWeapon::SpawnProjectile(const FHitResult& HitResult, const FVector& EndLocation,
                                          const FName& SocketName,
                                          const TSubclassOf<AKT_BaseProjectile>& InProjectileClass,
                                          const int32& InDamage)
{
	const FTransform LSpawnTransform = Mesh->GetSocketTransform(SocketName);
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


TSubclassOf<UDamageType> AKT_BaseRangeWeapon::GetDamageType(const FHitResult& InHit) const
{
	if (!Cast<AKT_PlayerCharacter>(InHit.Actor)) return BodyDamageType;
	
	if (InHit.BoneName == Cast<AKT_PlayerCharacter>(InHit.Actor)->HeadBoneName)
	{
		return HeadDamageType;
	}
	return BodyDamageType;
}


void AKT_BaseRangeWeapon::ToReload_Implementation()
{
	if (IsChangingFireMode || !IsValid(Character) || IsReloading) return;

	if (ClipReload)
	{
		ClipReloading();
	}
	else
	{
		StartBulletReloading();
	}
}


void AKT_BaseRangeWeapon::Reload(const int InAmmo)
{
	AmmoInTheClip += InAmmo;
	Character->ItemsManagerComponent->RemoveAmmoOnServer(GetClass(), InAmmo);
	
	if (ClipReload)
	{
		IsReloading = false;
		return;
	}
	if (AmmoInTheClip >= ClipSize)
	{
		EndBulletReloading();
		return;
	}

	if (AmmoInTheClip == 1 && NeedSendingOneBullet)
	{
		SendBullet();
		return;
	}
	OneBulletReloading();
}


void AKT_BaseRangeWeapon::ClipReloading()
{
	int32 LCountOfAmmo, LClipSize = ClipSize;
	LClipSize++;
	
	if (Character->IsScoping)
	{
		Character->UnScope_Implementation();
	}
	if (!Character->ItemsManagerComponent->CountAmmo(GetClass(), LCountOfAmmo) || AmmoInTheClip >= LClipSize) return;

	PlayAnimation(ReloadAnimation);
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


void AKT_BaseRangeWeapon::StartBulletReloading()
{
	int32 LCountOfAmmo;
	if (!Character->ItemsManagerComponent->CountAmmo(GetClass(), LCountOfAmmo) || AmmoInTheClip >= ClipSize) return;

	IsReloading = true;
	FTimerDelegate LTimerDelegate;
	LTimerDelegate.BindUFunction(this, "OneBulletReloading");
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, LTimerDelegate, StartReloadTime / Character->BerserkBooster,
									false);

	PlayAnimation(StartReloadingAnimation);
}


void AKT_BaseRangeWeapon::OneBulletReloading()
{
	IsSendingBullet = false;
	int32 LCountOfAmmo;
	if (!Character->ItemsManagerComponent->CountAmmo(GetClass(), LCountOfAmmo) || AmmoInTheClip >= ClipSize)
	{
		EndBulletReloading();
		return;
	}

	IsReloading = true;
	IsOneBulletReloading = true;
	PlayAnimation(ReloadAnimation);
	
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadTimerDelegate, ReloadTime / Character->BerserkBooster,
									false);
}


void AKT_BaseRangeWeapon::EndBulletReloading()
{
	IsSendingBullet = false;
	IsOneBulletReloading = false;
	IsEndingReload = true;
	IsReloading = false;
	
	PlayAnimation(EndReloadingAnimation);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	
	FTimerDelegate LTimerDelegate;
	LTimerDelegate.BindUFunction(this, "StopReloading");
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, LTimerDelegate, EndReloadTime / Character->BerserkBooster,
								false);
}


void AKT_BaseRangeWeapon::SendBullet()
{
	IsOneBulletReloading = false;
	
	IsSendingBullet = true;
	PlayAnimation(SendBulletAnimation);
	FTimerDelegate LTimerDelegate;
	LTimerDelegate.BindUFunction(this, "OneBulletReloading");
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, LTimerDelegate, SendBulletTime / Character->BerserkBooster,
								false);
}


void AKT_BaseRangeWeapon::StopReloading()
{
	IsEndingReload = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
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


void AKT_BaseRangeWeapon::SpawnMuzzleFlash_Implementation(UParticleSystem* MuzzleParticle, const FName& InShotSocketName)
{
	const FTransform LSocketTransform = Mesh->GetSocketTransform(InShotSocketName);
	UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, Mesh, InShotSocketName, LSocketTransform.GetLocation(),
		LSocketTransform.GetRotation().Rotator(), FVector::OneVector, EAttachLocation::KeepWorldPosition,
		true, EPSCPoolMethod::None, true);
}


void AKT_BaseRangeWeapon::PlayAnimation_Implementation(UAnimationAsset* InAnimation)
{
	if (HasAuthority()) return;
	
	Mesh->PlayAnimation(InAnimation, false);
}