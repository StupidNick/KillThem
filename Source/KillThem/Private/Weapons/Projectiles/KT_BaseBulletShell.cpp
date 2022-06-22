#include "Weapons/Projectiles/KT_BaseBulletShell.h"



AKT_BaseBulletShell::AKT_BaseBulletShell()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	
	RootComponent = Mesh;

	// Mesh->SetCollisionProfileName(FName("IgnoreAll"));
	Mesh->SetSimulatePhysics(true);
}


void AKT_BaseBulletShell::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle LTimerHandle;
	FTimerDelegate LTimerDelegate;
	LTimerDelegate.BindUFunction(this, "Destruction");
	GetWorldTimerManager().SetTimer(LTimerHandle, LTimerDelegate, DestructionTimer, false);
}


void AKT_BaseBulletShell::Destruction()
{
	Destroy();
}
