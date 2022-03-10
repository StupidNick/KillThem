#include "InteractiveObjects/KT_BaseInteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "Character/KT_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"


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


void AKT_BaseInteractiveObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKT_BaseInteractiveObject, PlayerCharacter);
	DOREPLIFETIME(AKT_BaseInteractiveObject, CanTake);
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
		InteractiveOnServer(LCharacter);
	}
}


void AKT_BaseInteractiveObject::OnSphereComponentBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		LCharacter->InteractInfoOnServer(this);
	}
}


void AKT_BaseInteractiveObject::OnSphereComponentEndOverlap_Implementation(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AKT_PlayerCharacter* LCharacter = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		PlayerCharacter = nullptr;
		LCharacter->UnInteractInfo();
	}
}


void AKT_BaseInteractiveObject::ToEnableObject(AKT_PlayerCharacter* Player)
{
	EnableObject(Player);
}


void AKT_BaseInteractiveObject::EnableObject(AKT_PlayerCharacter* Player)
{
	InteractSphereCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetGenerateOverlapEvents(true);
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetVisibility(true, true);
	}
	CanTake = true;

	// if (HasAuthority())
	// {
	// 	TArray<AActor*> LOverlappingActors;
	// 	InteractSphereCollision->GetOverlappingActors(LOverlappingActors);
	//
	// 	for (auto i : LOverlappingActors)
	// 	{
	// 		if (i->IsA(Player->GetClass()))
	// 		{
	// 			Cast<AKT_PlayerCharacter>(i)->InteractInfoOnServer(this);
	// 		}
	// 	}
	// }
}


void AKT_BaseInteractiveObject::DisableObject()
{
	InteractSphereCollision->SetGenerateOverlapEvents(false);
	BoxCollision->SetGenerateOverlapEvents(false);
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetVisibility(false, true);
	}
	CanTake = false;
}


void AKT_BaseInteractiveObject::InteractiveOnServer_Implementation(AKT_PlayerCharacter* Player)
{
	PlayerCharacter = Player;
}


void AKT_BaseInteractiveObject::InteractiveOnClient_Implementation(AKT_PlayerCharacter* Player)
{
}