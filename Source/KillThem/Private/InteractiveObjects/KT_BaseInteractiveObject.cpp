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
	StaticMesh->SetCollisionProfileName(FName("IgnoreAll"));

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

	if (!InteractOnPressButton)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnBoxComponentBeginOverlap);
	}
	else
	{
		InteractSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnSphereComponentBeginOverlap);
		InteractSphereCollision->OnComponentEndOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnSphereComponentEndOverlap);
	}
	if (RotateObject)
	{
		if (!HasAuthority())
		{
			RotationTimerDelegate.BindUFunction(this, "RotationObject");
			RotationObject();
		}
	}
}


void AKT_BaseInteractiveObject::RotationObject()
{
	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + 0.5, GetActorRotation().Roll));
	GetWorldTimerManager().SetTimer(RotationTimerHandle, RotationTimerDelegate, RotationTime, false);
}


void AKT_BaseInteractiveObject::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		ToInteractive(LCharacter);
		if (RecoverTime > 0)
		{
			EnableTimerDelegate.BindUFunction(this, "EnableObject");
			GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
		}
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
	InteractSphereCollision->SetGenerateOverlapEvents(false);
	BoxCollision->SetGenerateOverlapEvents(false);
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


void AKT_BaseInteractiveObject::ToInteractive(AKT_PlayerCharacter* Player)
{
	if (CanTake)
	{
		if (InteractOnPressButton)
		{
			Interactive(Player);
		}
		else
		{
			Interactive(Player);
		}
	}
}


void AKT_BaseInteractiveObject::Interactive(AKT_PlayerCharacter* Player)
{
}


void AKT_BaseInteractiveObject::InteractiveOnServer_Implementation(AKT_PlayerCharacter* Player)
{
	Interactive(Player);
}