#include "InteractiveObjects/Health/KT_BaseBooster.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/KT_HealthComponent.h"


AKT_BaseBooster::AKT_BaseBooster()
{
	RotationTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("SlidingTimeline"));
	RotationInterpFunction.BindUFunction(this, FName("RotationTimeLineFloatReturn"));
}


void AKT_BaseBooster::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloatForRotation)
	{
		RotationTimeLine->AddInterpFloat(CurveFloatForRotation, RotationInterpFunction, FName("Alpha"));
		RotationTimeLine->SetLooping(true);
		RotationTimeLine->PlayFromStart();
	}
}


void AKT_BaseBooster::RotationTimeLineFloatReturn(float Value)
{
	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + RotationSpeed, GetActorRotation().Roll));
}


///////////////////////////////////////////////////////////
void AKT_BaseBooster::HealthRecovery(AActor* OtherActor) const
{
	if (const AKT_PlayerCharacter* LPlayer = Cast<AKT_PlayerCharacter>(OtherActor))
	{
		LPlayer->HealthComponent->ChangeHealthOnServer(Health);
	}
}
////////////////////////////////////////////////////////////


void AKT_BaseBooster::Interactive(AActor* OtherActor)
{
	HealthRecovery(OtherActor);

	if (HasAuthority())
	{ 
		BoxCollision->SetGenerateOverlapEvents(false);
		DisableObject();
	}
}