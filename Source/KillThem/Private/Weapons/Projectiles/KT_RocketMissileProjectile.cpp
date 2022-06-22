#include "Weapons/Projectiles/KT_RocketMissileProjectile.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/SphereComponent.h"


void AKT_RocketMissileProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle LTimerHandle;
	FTimerDelegate LTimerDelegate;

	LTimerDelegate.BindUFunction(this, "SpawnMicroRockets");
	GetWorldTimerManager().SetTimer(LTimerHandle, LTimerDelegate, DelayBeforeSpawn, false);
}


void AKT_RocketMissileProjectile::SpawnMicroRockets()//TODO refactor
{
	// if (!IsOnServer) return;
	if (!HasAuthority()) return;
	
	FRotator LSpawnRotator = FRotator::ZeroRotator;
	for (int32 Counter = 0; Counter < CountOfMissile; Counter++)
	{
		FTransform LSpawnTransform = Mesh->GetSocketTransform("SpawnSocket");
		LSpawnTransform.SetRotation(FQuat(LSpawnTransform.Rotator()) + FQuat(LSpawnRotator));
		AKT_BaseProjectile* LProjectile = GetWorld()->SpawnActorDeferred<AKT_BaseProjectile>(
			ProjectileClass, LSpawnTransform);
		if (IsValid(LProjectile))
		{
			IgnoreProjectilesArray.Add(LProjectile);
			for (const auto Missile : IgnoreProjectilesArray)
			{
				Missile->CollisionComponent->MoveIgnoreActors.Append(IgnoreProjectilesArray);
			}
			const FVector LEndPoint = LSpawnTransform.GetLocation() + LSpawnTransform.GetRotation().GetForwardVector() * 100.0f;
			const FVector LDirection = (LEndPoint - Mesh->GetSocketLocation("SpawnSocket")).GetSafeNormal();
			LProjectile->SetShootDirection(LDirection);
			LProjectile->Initialize(Damage, PlayerOwner, WeaponInstigator);
			LProjectile->FinishSpawning(LSpawnTransform);
		}
		LSpawnRotator += FRotator(0.0f, 10.0f,0.0f);
	}
	Destroy();
	SpawnHitEffect(DestroyParticleSystem);
}