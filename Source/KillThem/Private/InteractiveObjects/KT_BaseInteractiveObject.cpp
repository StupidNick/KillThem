#include "InteractiveObjects/KT_BaseInteractiveObject.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseInteractiveObject::AKT_BaseInteractiveObject()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	StaticMesh->SetCollisionProfileName(FName("OverlapAll"));
}




void AKT_BaseInteractiveObject::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnComponentOverlap);
}

void AKT_BaseInteractiveObject::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Interactive(OtherActor);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetVisibility(false);

	EnableTimerDelegate.BindUFunction(this, "EnableObject");
    GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
}


void AKT_BaseInteractiveObject::EnableObject() const
{
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetVisibility(true);
}


void AKT_BaseInteractiveObject::Interactive(AActor* OtherActor)
{
}

void AKT_BaseInteractiveObject::InteractiveOnServer_Implementation(AActor* OtherActor)
{
	Interactive(OtherActor);
}
