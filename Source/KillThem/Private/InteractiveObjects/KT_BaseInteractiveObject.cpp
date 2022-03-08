#include "InteractiveObjects/KT_BaseInteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "Character/KT_PlayerCharacter.h"


AKT_BaseInteractiveObject::AKT_BaseInteractiveObject()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	InteractSphereCollision = CreateDefaultSubobject<USphereComponent>("InteractSphereCollision");
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StandStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StandStaticMesh");
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RotationTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotationTimeline"));

	RootComponent = SceneComponent;
	BoxCollision->SetupAttachment(SceneComponent);
	InteractSphereCollision->SetupAttachment(SceneComponent);
	StaticMesh->SetupAttachment(SceneComponent);
	StandStaticMesh->SetupAttachment(SceneComponent);

	StaticMesh->SetIsReplicated(true);
	
	RotationInterpFunction.BindUFunction(this, FName("RotationTimeLineFloatReturn"));
	
	StaticMesh->SetCollisionProfileName(FName("IgnoreAll"));
	StandStaticMesh->SetCollisionProfileName(FName("IgnoreAll"));

	BoxCollision->SetCollisionProfileName(FName("InteractiveObject"));
	InteractSphereCollision->SetCollisionProfileName(FName("InteractiveObject"));
}


void AKT_BaseInteractiveObject::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!InteractOnPressButton)
		{
			BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnBoxComponentBeginOverlap);
		}
		else
		{
			InteractSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnSphereComponentBeginOverlap);
			InteractSphereCollision->OnComponentEndOverlap.AddDynamic(this, &AKT_BaseInteractiveObject::OnSphereComponentEndOverlap);
		}
	}
	
	if (!HasAuthority())
	{
		if (RotationCurve)
		{
			RotationTimeLine->AddInterpFloat(RotationCurve, RotationInterpFunction, FName("Alpha"));
			RotationTimeLine->SetLooping(true);
			RotationTimeLine->PlayFromStart();
		}
	}
}


void AKT_BaseInteractiveObject::RotationTimeLineFloatReturn(float Value)
{
	StaticMesh->SetRelativeRotation(FRotator(StaticMesh->GetRelativeRotation().Pitch,
		StaticMesh->GetRelativeRotation().Yaw + SpeedRotation, StaticMesh->GetRelativeRotation().Roll));
}


void AKT_BaseInteractiveObject::OnBoxComponentBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		ToInteractive(LCharacter);
	}
}


void AKT_BaseInteractiveObject::OnSphereComponentBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		LCharacter->InteractInfo(this);
	}
}


void AKT_BaseInteractiveObject::OnSphereComponentEndOverlap_Implementation(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		LCharacter->UnInteractInfo();
	}
}


void AKT_BaseInteractiveObject::ToEnableObject_Implementation()
{
	EnableObject();
}


void AKT_BaseInteractiveObject::EnableObject_Implementation()
{
	InteractSphereCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetGenerateOverlapEvents(true);
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetVisibility(true, true);
	}
	CanTake = true;
}


void AKT_BaseInteractiveObject::DisableObject_Implementation()
{
	InteractSphereCollision->SetGenerateOverlapEvents(false);
	BoxCollision->SetGenerateOverlapEvents(false);
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetVisibility(false, true);
	}
	CanTake = false;
	if (RecoverTime > 0)
	{
		EnableTimerDelegate.BindUFunction(this, "ToEnableObject");
		GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
	}
}


void AKT_BaseInteractiveObject::ToInteractive_Implementation(AKT_PlayerCharacter* Player)
{
	if (CanTake)
	{
		Interactive(Player);
	}
}


void AKT_BaseInteractiveObject::Interactive_Implementation(AKT_PlayerCharacter* Player)
{
	PlayerCharacter = Player;
	DisableObject();
}