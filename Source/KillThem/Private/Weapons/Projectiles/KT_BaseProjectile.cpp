#include "Weapons/Projectiles/KT_BaseProjectile.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AKT_BaseProjectile::AKT_BaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = CollisionComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(CollisionComponent);

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
	
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	if (IsRadialDamage)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(),
			DamageRadius, UDamageType::StaticClass(), {GetOwner()}, this,
			PlayerOwner->Controller, DoFullDamage);
		DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5);
	}
	else
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, PlayerOwner->PlayerController,
			PlayerOwner, UDamageType::StaticClass());
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


void AKT_BaseProjectile::SetShootDirection(const FVector& Direction)
{
	ShootDirection = Direction;
}
