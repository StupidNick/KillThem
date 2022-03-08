#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"

#include "Character/KT_PlayerCharacter.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"


AKT_BaseLyingObject::AKT_BaseLyingObject()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");

	SkeletalMesh->SetupAttachment(SceneComponent);
	SkeletalMesh->SetIsReplicated(true);

	BoosterTimeLineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimerTimeline"));

	BoosterInterpFunction.BindUFunction(this, FName("TimeLineProgress"));

	bReplicates = true;
}


void AKT_BaseLyingObject::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BoosterIndicateCurveFloat) && !HasAuthority())
	{
		BoosterTimeLineComponent->AddInterpFloat(BoosterIndicateCurveFloat, BoosterInterpFunction, FName("Alpha"));
		BoosterTimeLineComponent->SetLooping(false);
	}
}


void AKT_BaseLyingObject::TimeLineProgress_Implementation(float Value)
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->OnBoosterUpdates.Broadcast(BoosterTimeLineComponent->GetPlaybackPosition());
}


void AKT_BaseLyingObject::RotationTimeLineFloatReturn(float Value)
{
	SkeletalMesh->SetRelativeRotation(FRotator(SkeletalMesh->GetRelativeRotation().Pitch,
		SkeletalMesh->GetRelativeRotation().Yaw + SpeedRotation, SkeletalMesh->GetRelativeRotation().Roll));
}


void AKT_BaseLyingObject::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);
	
	if (IsABooster)
	{
		BoosterActivated(Player, BoosterIcon, BoosterCoolingTime);
		
		BoostDownTimerDelegate.BindUFunction(this, "BoostDown", Player);
		GetWorldTimerManager().SetTimer(BoostDownTimerHandle, BoostDownTimerDelegate, BoosterCoolingTime, false);
	}
}


void AKT_BaseLyingObject::BoosterActivated_Implementation(AKT_PlayerCharacter* Player, UTexture2D* Icon, float Time)
{
	PlayerCharacter = Player;
	if (!IsValid(PlayerCharacter) || HasAuthority()) return;

	BoosterTimeLineComponent->SetPlaybackPosition(Time, true);
	BoosterTimeLineComponent->Reverse();
	
	PlayerCharacter->OnBoosterActivated.Broadcast(Icon, Time);
}


void AKT_BaseLyingObject::BoosterDeactivated_Implementation()
{
	if (!IsValid(PlayerCharacter)) return;
	
	PlayerCharacter->OnBoosterDeactivated.Broadcast(true);
}


void AKT_BaseLyingObject::DisableObject()
{
	Super::DisableObject();

	if (IsValid(SkeletalMesh))
	{
		SkeletalMesh->SetVisibility(false, true);
	}
}


void AKT_BaseLyingObject::BoostDown(AKT_PlayerCharacter* Player) 
{
	BoosterDeactivated();
	PlayerCharacter = nullptr;
}


void AKT_BaseLyingObject::EnableObject()
{
	Super::EnableObject();

	if (IsValid(SkeletalMesh))
	{
		SkeletalMesh->SetVisibility(true, true);
	}
}
