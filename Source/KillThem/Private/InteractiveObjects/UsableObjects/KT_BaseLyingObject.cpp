#include "InteractiveObjects/UsableObjects/KT_BaseLyingObject.h"

#include "Character/KT_PlayerCharacter.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"


AKT_BaseLyingObject::AKT_BaseLyingObject()
{
	TimerTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimerTimeline"));

	TimerInterpFunction.BindUFunction(this, FName("TimerTimeLineFloatReturn"));
}


void AKT_BaseLyingObject::BeginPlay()
{
	Super::BeginPlay();

	if (TimerCurveFloat)
	{
		TimerTimeLine->AddInterpFloat(TimerCurveFloat, TimerInterpFunction, FName("Alpha"));
		TimerTimeLine->SetLooping(false);
	}
}


void AKT_BaseLyingObject::TimerTimeLineFloatReturn_Implementation(float Value)
{
	if (Character)
	{
		Character->OnTimeBustedUpdate.Broadcast(TimerTimeLine->GetPlaybackPosition());
	}
}


void AKT_BaseLyingObject::Interactive(AKT_PlayerCharacter* Player)
{
	Character = Player;
	DisableObject();
	if (IsABooster)
	{
		Player->VisibleBooster(BoosterIcon, BoostDownTimer);
		
		BoostDownTimerDelegate.BindUFunction(this, "BoostDown", Player);
		GetWorldTimerManager().SetTimer(BoostDownTimerHandle, BoostDownTimerDelegate, BoostDownTimer, false);
		
		TimerTimeLine->SetPlaybackPosition(BoostDownTimer, true);
		TimerTimeLine->Reverse();
	}
}

void AKT_BaseLyingObject::BoostDown(AKT_PlayerCharacter* Player) 
{
	Player->DisableBooster();
	Character = nullptr;
}
