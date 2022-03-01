#include "Components/KT_ItemsManagerComponent.h"

#include "Character/KT_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"


UKT_ItemsManagerComponent::UKT_ItemsManagerComponent()
{
}


void UKT_ItemsManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UKT_ItemsManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKT_ItemsManagerComponent, FirstWeaponSlot);
	DOREPLIFETIME(UKT_ItemsManagerComponent, SecondWeaponSlot);
	DOREPLIFETIME(UKT_ItemsManagerComponent, SelectedFirstSlot);
}


void UKT_ItemsManagerComponent::ChangeIcon_Implementation()
{
	OnWeaponChange.Broadcast(GetSelectedWeaponSlot()->WeaponIcon, GetSelectedWeaponSlot()->AimIcon);
}


void UKT_ItemsManagerComponent::ChangeAmmoInTheClip_Implementation(int Ammo)
{
	OnAmmoInTheClipChange.Broadcast(Ammo);
}


void UKT_ItemsManagerComponent::AmountOfAmmoChanged_Implementation(const int Ammo)
{
	OnAmmoChangeBind.Broadcast(Ammo);
}


void UKT_ItemsManagerComponent::OnRep_WeaponChanged_Implementation()
{
	if (IsValid(GetSelectedWeaponSlot()))
	{
		int LAmountAmmo;
		OnAmmoInTheClipChange.Broadcast(GetSelectedWeaponSlot()->GetAmmoInTheClip());
		if (FindAndCountAmmo(GetSelectedWeaponSlot()->GetClass(), LAmountAmmo))
		{
			OnAmmoChangeBind.Broadcast(LAmountAmmo);
		}
		OnWeaponChange.Broadcast(GetSelectedWeaponSlot()->WeaponIcon, GetSelectedWeaponSlot()->AimIcon);
	}
}


void UKT_ItemsManagerComponent::AddAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmoFound)
{
	if (FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		LAmmoStruct->CountOfAmmo += InNumberOfAmmoFound;
		if (LAmmoStruct->CountOfAmmo > LAmmoStruct->MaxAmmo)
		{
			LAmmoStruct->CountOfAmmo = LAmmoStruct->MaxAmmo;
		}
		if (GetSelectedWeaponSlot()->GetClass() == LAmmoStruct->TypeOfAmmo)
		{
			AmountOfAmmoChanged(LAmmoStruct->CountOfAmmo);
		}
	}
}


bool UKT_ItemsManagerComponent::FindAndCountAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass,
                                                 int& InNumberOfAmmo)
{
	if (const FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		InNumberOfAmmo = LAmmoStruct->CountOfAmmo;
		return true;
	}
	return false;
}


bool UKT_ItemsManagerComponent::RemoveAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmo)
{
	if (FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		if (LAmmoStruct->CountOfAmmo >= InNumberOfAmmo)
		{
			LAmmoStruct->CountOfAmmo -= InNumberOfAmmo;
			AmountOfAmmoChanged(LAmmoStruct->CountOfAmmo);
			return true;
		}
	}
	return false;
}


void UKT_ItemsManagerComponent::Initialize(AKT_PlayerCharacter* InCharacter)
{
	PlayerCharacter = InCharacter;
}


void UKT_ItemsManagerComponent::ChangeWeapon_Implementation()
{
	if (GetSelectedWeaponSlot() == FirstWeaponSlot && IsValid(SecondWeaponSlot))
	{
		FirstWeaponSlot->CanShoot = false;
		GetWorld()->GetTimerManager().ClearAllTimersForObject(FirstWeaponSlot);
		SecondWeaponSlot->CanShoot = true;
		SelectedFirstSlot = false;
	}
	else if (GetSelectedWeaponSlot() == SecondWeaponSlot && IsValid(FirstWeaponSlot))
	{
		SecondWeaponSlot->CanShoot = false;
		GetWorld()->GetTimerManager().ClearAllTimersForObject(SecondWeaponSlot);
		FirstWeaponSlot->CanShoot = true;
		SelectedFirstSlot = true;
	}
}
