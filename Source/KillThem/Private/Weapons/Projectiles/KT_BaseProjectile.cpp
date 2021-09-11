#include "Weapons/Projectiles/KT_BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/KT_PlayerCharacter.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AKT_BaseProjectile::AKT_BaseProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseProjectile::OnOverlap);
}


void AKT_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_BaseProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor == PlayerOwner || OtherActor == this)
	{
		return;
	}
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	if (OtherActor->IsA<AKT_PlayerCharacter>())
	{
		const FDamageEvent LDamageEvent;
		const TSubclassOf<UDamageType> LDamageTypeClass;
		
		UGameplayStatics::ApplyDamage(OtherActor, Damage, PlayerOwner->PlayerController, PlayerOwner, LDamageTypeClass);
	}
	Destroy();
}


void AKT_BaseProjectile::Initialize(const float InDamage, AKT_PlayerCharacter* InPlayerOwner, AKT_BaseRangeWeapon* InWeaponInstigator)
{
	Damage = InDamage;
	PlayerOwner = InPlayerOwner;
	WeaponInstigator = InWeaponInstigator;
}


