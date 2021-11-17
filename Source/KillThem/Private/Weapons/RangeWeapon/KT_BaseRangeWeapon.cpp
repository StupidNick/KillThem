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
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(Mesh);
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
	
		UE_LOG(LogTemp, Error, TEXT("%i"), AmmoInTheClip);
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
	if (IsScoping)
	{
		UnScope();
	}
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


void AKT_BaseRangeWeapon::Scope_Implementation()
{
	if (!IsReloading)
	{
		IsScoping = true;
	}
}


void AKT_BaseRangeWeapon::UnScope()
{
	IsScoping = false;
}


void AKT_BaseRangeWeapon::OnRep_Scoping_Implementation()
{
	if (IsScoping)
	{
		CameraComponent->SetActive(true);
		Character->CameraComponent->SetActive(false, true);
		SetScatterFactor(ScopeScatterFactor);
	}
	else
	{
		CameraComponent->SetActive(false);
		Character->CameraComponent->SetActive(true);
		SetScatterFactor(BaseScatterFactor);
	}
}


void AKT_BaseRangeWeapon::SetScatterFactor(const float InScatterFactor)
{
	ScatterFactor = InScatterFactor;
}
