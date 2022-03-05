#include "UI/MainHUD_WD/KT_MainHUD_WD.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/KT_HealthComponent.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Components/ProgressBar.h"
#include "UI/MainHUD_WD/Panels/KT_AimWD.h"
#include "UI/MainHUD_WD/Panels/KT_IconWithTextWD.h"


UKT_MainHUD_WD::UKT_MainHUD_WD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}


void UKT_MainHUD_WD::InitializeMainHUD_Implementation(AKT_PlayerCharacter* PlayerCharacter)
{
	Character = PlayerCharacter;

	if (!Character)
	{
		return;
	}

	Ability->SetVisibility(ESlateVisibility::Hidden);
	
	Character->HealthComponent->OnHPChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateHP);
	UpdateHP(Character->HealthComponent->GetHealth());

	Character->HealthComponent->OnSPChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateSP);
	UpdateSP(Character->HealthComponent->GetShield());

	Character->ItemsManagerComponent->OnHandWeaponAmmoChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateAmmo);
	UpdateAmmo(Character->ItemsManagerComponent->AmmoForFirstWeapon);
	
	Character->ItemsManagerComponent->OnAmmoInTheClipChange.AddDynamic(this, &UKT_MainHUD_WD::UpdateAmmoInTheClip);
	// UpdateAmmoInTheClip(Character->ItemsManagerComponent->GetSelectedWeaponSlot()->GetAmmoInTheClip());
	
	Character->ItemsManagerComponent->OnWeaponChange.AddDynamic(this, &UKT_MainHUD_WD::UpdateIcon);
	
	Character->OnTimeBustedUpdate.AddDynamic(this, &UKT_MainHUD_WD::UpdateBooster);
}


void UKT_MainHUD_WD::UpdateBooster_Implementation(float Timer)
{
	Ability->ValueTextBlock->SetText(FText::FromString(FString::FromInt(Timer)));
	Ability->ValueProgressBar->SetPercent(Timer/Ability->TimerTime);
}


void UKT_MainHUD_WD::UpdateHP_Implementation(float HPStat)
{
	HPBar->ValueProgressBar->SetPercent(HPStat / 100);
	HPBar->ValueTextBlock->SetText(FText::FromString(FString::FromInt(HPStat)));
}


void UKT_MainHUD_WD::UpdateSP_Implementation(float SPStat)
{
	SPBar->ValueProgressBar->SetPercent(SPStat / 100);
	SPBar->ValueTextBlock->SetText(FText::FromString(FString::FromInt(SPStat)));
}


void UKT_MainHUD_WD::UpdateAmmo_Implementation(int InAmmo)
{
	WeaponBar->AmmoTextBlock->SetText(FText::FromString(FString::FromInt(InAmmo)));
}


void UKT_MainHUD_WD::UpdateAmmoInTheClip_Implementation(int InAmmo)
{
	WeaponBar->AmmoInTheClipTextBlock->SetText(FText::FromString(FString::FromInt(InAmmo)));
}


void UKT_MainHUD_WD::UpdateIcon_Implementation(UTexture2D* InWeaponIcon, UTexture2D* InAimIcon)
{
	WeaponBar->Icon->SetBrushFromTexture(InWeaponIcon);
	Aim->Aim->SetBrushFromTexture(InAimIcon);
}