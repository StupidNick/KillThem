#include "UI/MainHUD_WD/KT_MainHUD_WD.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_HealthComponent.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "Components/ProgressBar.h"
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

	Character->HealthComponent->OnHPChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateHP);
	UpdateHP(Character->HealthComponent->GetHealth());

	Character->HealthComponent->OnSPChangeBind.AddDynamic(this, &UKT_MainHUD_WD::UpdateSP);
	UpdateSP(Character->HealthComponent->GetShield());

	// Character->ItemsManagerComponent->OnCountOfAmmoChange.AddDynamic(this, &UKT_MainHUD_WD::UpdateAmmo);
	// UpdateAmmo(1, Character->ItemsManagerComponent->GetCountOfAmmoForWeaponInHand());
	
}


void UKT_MainHUD_WD::UpdateHP_Implementation(float HPStat)
{
	HPBar->ValueProgressBar->SetPercent(HPStat / 100);
	HPBar->ValueTextBlock->SetText(FText::FromString(FString::SanitizeFloat(HPStat)));
}


void UKT_MainHUD_WD::UpdateSP_Implementation(float SPStat)
{
	SPBar->ValueProgressBar->SetPercent(SPStat / 100);
	SPBar->ValueTextBlock->SetText(FText::FromString(FString::SanitizeFloat(SPStat)));
}


void UKT_MainHUD_WD::UpdateAmmo_Implementation(const int ClipAmmo, const int Ammo)
{
	FString LAmmoString = FString::FromInt(ClipAmmo);
	LAmmoString.Append(Delimiter);
	LAmmoString.Append(FString::FromInt(Ammo));
	const FText LAmmoText = FText::FromString(LAmmoString);

	WeaponBar->TextBlock->SetText(LAmmoText);
}


void UKT_MainHUD_WD::UpdateIcon_Implementation(const UImage* InIcon)
{
}