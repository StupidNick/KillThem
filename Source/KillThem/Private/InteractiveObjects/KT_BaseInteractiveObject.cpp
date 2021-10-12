#include "InteractiveObjects/KT_BaseInteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseInteractiveObject::AKT_BaseInteractiveObject()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	InteractSphereCollision = CreateDefaultSubobject<USphereComponent>("InteractSphereCollision");
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
		
	RootComponent = SkeletalMesh;
	BoxCollision->SetupAttachment(SkeletalMesh);
	InteractSphereCollision->SetupAttachment(SkeletalMesh);
	StaticMesh->SetupAttachment(SkeletalMesh);
	SkeletalMesh->SetCollisionProfileName(FName("IgnoreAll"));
	


	BoxCollision->SetCollisionProfileName(FName("InteractiveObject"));
	InteractSphereCollision->SetCollisionProfileName(FName("InteractiveObject"));
}




void AKT_BaseInteractiveObject::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(StaticMesh->GetStaticMesh()))
	{
		StaticMesh->DestroyComponent();
	}
	else if (!IsValid(SkeletalMesh->SkeletalMesh))
	{
		SkeletalMesh->DestroyComponent();
		RootComponent = StaticMesh;
	}

	if (!CanInteractOnOverlap)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnBoxComponentBeginOverlap);
	}
	else
	{
		InteractSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnSphereComponentBeginOverlap);
		InteractSphereCollision->OnComponentEndOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnSphereComponentEndOverlap);
	}
}


void AKT_BaseInteractiveObject::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ToInteractive(OtherActor);
	if (RecoverTime > 0)
	{
		EnableTimerDelegate.BindUFunction(this, "EnableObject");
		GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
	}
}


void AKT_BaseInteractiveObject::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		LCharacter->InteractInfo(this);
	}
}


void AKT_BaseInteractiveObject::OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		LCharacter->UnInteractInfo();
	}
}



void AKT_BaseInteractiveObject::EnableObject()
{
	InteractSphereCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetGenerateOverlapEvents(true);
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetVisibility(true);
	}
	else if (IsValid(SkeletalMesh))
	{
		SkeletalMesh->SetVisibility(true);
	}
	CanTake = true;
}


void AKT_BaseInteractiveObject::DisableObject()
{
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetVisibility(false);
	}
	else if (IsValid(SkeletalMesh))
	{
		SkeletalMesh->SetVisibility(false);
	}
	CanTake = false;
}


void AKT_BaseInteractiveObject::ToInteractive(AActor* OtherActor)
{
	if (CanTake)
	{
		if (HasAuthority())
		{
			InteractiveOnServer(OtherActor);
		}
		else
		{
			Interactive(OtherActor);
		}
	}
}


void AKT_BaseInteractiveObject::Interactive(AActor* OtherActor)
{
}


void AKT_BaseInteractiveObject::InteractiveOnServer_Implementation(AActor* OtherActor)
{
	Interactive(OtherActor);
}