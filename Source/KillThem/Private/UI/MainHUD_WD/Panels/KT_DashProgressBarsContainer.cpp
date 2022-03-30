#include "UI/MainHUD_WD/Panels/KT_DashProgressBarsContainer.h"

#include "Character/KT_PlayerCharacter.h"



UKT_DashProgressBarsContainer::UKT_DashProgressBarsContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HideTimerDelegate.BindUFunction(this, "HideContainer");
}


void UKT_DashProgressBarsContainer::InitializeProgressBarsContainer(const AKT_PlayerCharacter* Player)
{
	const UKT_CharacterMovementComponent* LCharacterMovementComponent = Cast<UKT_CharacterMovementComponent>(Player->GetMovementComponent());
	if (!IsValid(Player) || !IsValid(LCharacterMovementComponent) || !IsValid(HorizontalBox) || !ProgressBarClass) return;
	
	for (int32 LCounter = 0; LCounter < LCharacterMovementComponent->MaxNumberOfDashes; LCounter++)
	{
		UKT_DashProgressBar* LProgressBar = CreateWidget<UKT_DashProgressBar>(this, ProgressBarClass);
		if (IsValid(LProgressBar))
		{
			if (LCounter < LCharacterMovementComponent->StartNumberOfDashes)
			{
				LProgressBar->ValueProgressBar->SetPercent(100);
			}
			
			ProgressBars.Add(LProgressBar);
			HorizontalBox->AddChild(LProgressBar);
			LProgressBar->SetPadding(FMargin(ProgressBarPaddings,0,ProgressBarPaddings,0));
		}
	}
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, HideTimerDelegate, 5, false);
}


void UKT_DashProgressBarsContainer::UpdateProgressBar(const float& Value, const int32& Index)
{
	if (!ProgressBars.IsValidIndex(Index)) return;
	
	ProgressBars[Index]->ValueProgressBar->SetPercent(Value);
}


void UKT_DashProgressBarsContainer::ClearProgressBar(const int32& Index)
{
	UnHideContainer();
	if (!ProgressBars.IsValidIndex(Index)) return;
	
	for (int32 LCounter = Index; LCounter < ProgressBars.Num(); LCounter++)
	{
		ProgressBars[LCounter]->ValueProgressBar->SetPercent(0);
	}
}


void UKT_DashProgressBarsContainer::HideContainer_Implementation()
{
	bIsVisible = false;
}


void UKT_DashProgressBarsContainer::UnHideContainer_Implementation()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, HideTimerDelegate, 5, false);
	
	bIsVisible = true;
}
