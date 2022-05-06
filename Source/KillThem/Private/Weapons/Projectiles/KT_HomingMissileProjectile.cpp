#include "Weapons/Projectiles/KT_HomingMissileProjectile.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"



AKT_HomingMissileProjectile::AKT_HomingMissileProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AKT_HomingMissileProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (StarterDelay > 0)
	{
		StarterDelay -= DeltaSeconds;
		return;
	}
	SetRotationToTarget();
	SetNewVelocity();
}


void AKT_HomingMissileProjectile::SetRotationToTarget() const
{
	FVector LStartLocation, LEndLocation;
	if (!GetTraceData(LStartLocation, LEndLocation)) return;

	FHitResult LHitResult;
	MakeHit(LHitResult, LStartLocation, LEndLocation);

	FRotator LRotationToTarget;
	if (LHitResult.IsValidBlockingHit())
	{
		LRotationToTarget = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), LHitResult.Location);
	}
	else
	{
		LRotationToTarget = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), LEndLocation);
	}
	const FRotator LInterpRotation = FMath::RInterpTo(GetActorRotation(), LRotationToTarget, GetWorld()->DeltaTimeSeconds, RotationSpeed);
	CollisionComponent->SetWorldRotation(LInterpRotation);
}

void AKT_HomingMissileProjectile::SetNewVelocity()
{
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, ProjectileMovementComponent->MaxSpeed, GetWorld()->DeltaTimeSeconds, AccelerationSpeed);
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(CurrentSpeed, 0,0));
}


bool AKT_HomingMissileProjectile::GetTraceData(FVector& StartLocation, FVector& EndLocation) const
{
	if (!IsValid(PlayerOwner) || !IsValid(PlayerOwner->Controller)) return false;
	
	FVector LViewLocation;
	FRotator LViewRotation;
	PlayerOwner->Controller->GetPlayerViewPoint(LViewLocation, LViewRotation);

	StartLocation = LViewLocation;
	EndLocation = StartLocation + MaxDistanceAttack * LViewRotation.Vector();
	return true;
}


void AKT_HomingMissileProjectile::MakeHit(FHitResult& HitResult, const FVector& StartLocation, const FVector& EndLocation) const
{
	FCollisionQueryParams LCollisionParams;

	LCollisionParams.AddIgnoredActor(this);
	LCollisionParams.AddIgnoredActor(PlayerOwner);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility,
										 LCollisionParams);
}