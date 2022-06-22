#include "Weapons/Projectiles/KT_BaseProjectile.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AKT_BaseProjectile::AKT_BaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = CollisionComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(CollisionComponent);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(Mesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseProjectile::OnOverlap);

	bReplicates = true;
	SetReplicateMovement(true);
}


void AKT_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(ProjectileMovementComponent);
	check(CollisionComponent);
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
	SetLifeSpan(ProjectileLifeTime);
}


void AKT_BaseProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!IsValid(PlayerOwner) || OtherActor == PlayerOwner || OtherActor == this) return;
	if (!IsOnServer)
	{
		if (IsRadialDamage)
		{
			SpawnHitEffect(DestroyParticleSystem);
		}
		ProjectileMovementComponent->StopMovementImmediately();
		Destroy();
		return;
	}
	
	if (IsRadialDamage)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(),
			DamageRadius, BodyDamageType, {}, this,
			PlayerOwner->Controller, DoFullDamage);
		DrawDebugSphere(GetWorld(), Hit.Location, DamageRadius, 5, FColor::Red, false, 5);
	}
	else if (OtherActor->IsA<AKT_PlayerCharacter>())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, PlayerOwner->PlayerController,
			PlayerOwner, GetDamageType(Hit));
	}
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	ProjectileMovementComponent->StopMovementImmediately();
	Destroy();
}


void AKT_BaseProjectile::Initialize(const float InDamage, AKT_PlayerCharacter* InPlayerOwner, AKT_BaseRangeWeapon* InWeaponInstigator)
{
	Damage = InDamage;
	PlayerOwner = InPlayerOwner;
	WeaponInstigator = InWeaponInstigator;
}


void AKT_BaseProjectile::HiddenProjectile_Implementation()
{
	Mesh->SetHiddenInGame(true);
	ParticleSystem->SetHiddenInGame(true);
}


void AKT_BaseProjectile::SetShootDirection(const FVector& Direction)
{
	ShootDirection = Direction;
}


void AKT_BaseProjectile::SpawnHitEffect_Implementation(UParticleSystem* InParticleSystem)
{
	// if (HasAuthority()) return;

	const FTransform LSocketTransform = Mesh->GetComponentTransform();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InParticleSystem, LSocketTransform.GetLocation(),
		LSocketTransform.GetRotation().Rotator(), FVector::OneVector,
		true, EPSCPoolMethod::None, true);
}


TSubclassOf<UDamageType> AKT_BaseProjectile::GetDamageType(const FHitResult& InHit) const
{
	if (!Cast<AKT_PlayerCharacter>(InHit.Actor)) return BodyDamageType;
	
	if (InHit.BoneName == Cast<AKT_PlayerCharacter>(InHit.Actor)->HeadBoneName)
	{
		return HeadDamageType;
	}
	return BodyDamageType;
}


void AKT_BaseProjectile::SetIsOnServer(bool Value)
{
	IsOnServer = Value;
}