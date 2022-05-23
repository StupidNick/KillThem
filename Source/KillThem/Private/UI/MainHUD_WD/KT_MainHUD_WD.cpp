#include "UI/MainHUD_WD/KT_MainHUD_WD.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/KT_HealthComponent.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Components/ProgressBar.h"
#include "UI/MainHUD_WD/Panels/KT_AimWD.h"
#include "UI/MainHUD_WD/Panels/KT_DashProgressBarsContainer.h"
#include "UI/MainHUD_WD/Panels/KT_IconWithTextWD.h"


UKT_MainHUD_WD::UKT_MainHUD_WD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}


void UKT_MainHUD_WD::InitializeMainHUD_Implementation(AKT_PlayerCharacter* PlayerCharacter)
{
	Character = PlayerCharacter;

	if (!IsValid(Character)) return;

	Ability->SetVisibility(ESlateVisibility::Hidden);
	
	Character->HealthComponent->OnHPChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateHP);
	UpdateHP(Character->HealthComponent->GetHealth());

	Character->HealthComponent->OnSPChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateSP);
	UpdateSP(Character->HealthComponent->GetShield());

	Character->ItemsManagerComponent->OnHandWeaponAmmoChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateAmmo);
	UpdateAmmo(Character->ItemsManagerComponent->AmmoForFirstWeapon);
	
	Character->ItemsManagerComponent->OnAmmoInTheClipChange.AddDynamic(this, &UKT_MainHUD_WD::UpdateAmmoInTheClip);
	Character->ItemsManagerComponent->OnWeaponChange.AddDynamic(this, &UKT_MainHUD_WD::UpdateIcon);
	if (IsValid(Character->ItemsManagerComponent->GetSelectedWeaponSlot()))
	{
		const auto LWeapon = Character->ItemsManagerComponent->GetSelectedWeaponSlot();
		UpdateAmmoInTheClip(LWeapon->GetAmmoInTheClip());
		UpdateIcon(LWeapon->WeaponIcon, LWeapon->AimIcon);
	}
	
	Character->OnBoosterActivated.AddDynamic(this, &UKT_MainHUD_WD::ActivateBooster);
	Character->OnBoosterDeactivated.AddDynamic(this, &UKT_MainHUD_WD::DeactivatedBooster);
	Character->OnBoosterUpdates.AddDynamic(this, &UKT_MainHUD_WD::UpdateBooster);
	
	Character->OnDashRecoverUpdates.AddDynamic(DashProgressBarContainer, &UKT_DashProgressBarsContainer::UpdateProgressBar);
	Character->OnDashUsed.AddDynamic(DashProgressBarContainer, &UKT_DashProgressBarsContainer::ClearProgressBar);
	DashProgressBarContainer->InitializeProgressBarsContainer(Character);
}


void UKT_MainHUD_WD::UpdateBooster_Implementation(float Timer)
{
	Ability->ValueTextBlock->SetText(FText::AsNumber(FMath::Floor(Timer)));
	Ability->ValueProgressBar->SetPercent(Timer/Ability->TimerValue);
}


void UKT_MainHUD_WD::ActivateBooster_Implementation(UTexture2D* Icon, float Time)
{
	Ability->BackgroundImage->SetBrushFromTexture(Icon);
	Ability->ValueTextBlock->SetText(FText::AsNumber(FMath::Floor(Time)));
	Ability->TimerValue = Time;
	Ability->SetVisibility(ESlateVisibility::Visible);
}


void UKT_MainHUD_WD::DeactivatedBooster_Implementation(bool Deactivated)
{
	Ability->SetVisibility(ESlateVisibility::Hidden);
}


void UKT_MainHUD_WD::UpdateHP_Implementation(float HPStat)
{
	HPBar->ValueProgressBar->SetPercent(HPStat / 100);
	HPBar->ValueTextBlock->SetText(FText::AsNumber(FMath::Floor(HPStat)));
}


void UKT_MainHUD_WD::UpdateSP_Implementation(float SPStat)
{
	SPBar->ValueProgressBar->SetPercent(SPStat / 100);
	SPBar->ValueTextBlock->SetText(FText::AsNumber(FMath::Floor(SPStat)));
}


void UKT_MainHUD_WD::UpdateAmmo_Implementation(int InAmmo)
{
	WeaponBar->AmmoTextBlock->SetText(FText::AsNumber(InAmmo));
}


void UKT_MainHUD_WD::UpdateAmmoInTheClip_Implementation(int InAmmo)
{
	WeaponBar->AmmoInTheClipTextBlock->SetText(FText::AsNumber(InAmmo));
}


void UKT_MainHUD_WD::UpdateIcon_Implementation(UTexture2D* InWeaponIcon, UTexture2D* InAimIcon)
{
	WeaponBar->Icon->SetBrushFromTexture(InWeaponIcon);
	Aim->Aim->SetBrushFromTexture(InAimIcon);
}