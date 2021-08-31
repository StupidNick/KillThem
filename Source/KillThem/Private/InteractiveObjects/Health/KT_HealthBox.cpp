#include "InteractiveObjects/Health/KT_HealthBox.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_HealthComponent.h"


AKT_HealthBox::AKT_HealthBox()
{
	RotationTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("SlidingTimeline"));
	RotationInterpFunction.BindUFunction(this, FName("RotationTimeLineFloatReturn"));
}


void AKT_HealthBox::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloatForRotation)
	{
		RotationTimeLine->AddInterpFloat(CurveFloatForRotation, RotationInterpFunction, FName("Alpha"));
		RotationTimeLine->SetLooping(true);
		RotationTimeLine->PlayFromStart();
	}
}


void AKT_HealthBox::RotationTimeLineFloatReturn(float Value)
{
	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + RotationSpeed, GetActorRotation().Roll));
}

void AKT_HealthBox::HealthRecovery(AActor* OtherActor)
{
	if (Cast<AKT_PlayerCharacter>(OtherActor))
	{
		Cast<AKT_PlayerCharacter>(OtherActor)->HealthComponent->HealthRecovery(Health);
	}
}


void AKT_HealthBox::Interactive(AActor* OtherActor)
{
	HealthRecovery(OtherActor);
}

