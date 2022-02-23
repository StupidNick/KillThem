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
	for (int i = 0; i < AmmoArray.Num(); i++)
	{
		if (AmmoArray[i].TypeOfAmmo == InAmmoClass)
		{
			AmmoArray[i].CountOfAmmo += InNumberOfAmmoFound;
			UE_LOG(LogTemp, Error, TEXT("%s: %i"), *GetSelectedWeaponSlot()->GetName(), AmmoArray[i].CountOfAmmo);
			AmountOfAmmoChanged(AmmoArray[i].CountOfAmmo);
			return;
		}
	}
}


bool UKT_ItemsManagerComponent::FindAndCountAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass,
                                                 int& InNumberOfAmmo)
{
	for (auto i : AmmoArray)
	{
		if (i.TypeOfAmmo == InAmmoClass && i.CountOfAmmo > 0)
		{
			InNumberOfAmmo = i.CountOfAmmo;
			return true;
		}
	}
	return false;
}


FAmmo UKT_ItemsManagerComponent::FindStructOfAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass)
{
	for (auto i : AmmoArray)
	{
		if (i.TypeOfAmmo == InAmmoClass)
		{
			return i;
		}
	}
	return AmmoArray.Top();
}


bool UKT_ItemsManagerComponent::RemoveAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmo)
{
	for (int i = 0; i < AmmoArray.Num(); i++)
	{
		if (AmmoArray[i].TypeOfAmmo == InAmmoClass && AmmoArray[i].CountOfAmmo >= InNumberOfAmmo)
		{
			AmmoArray[i].CountOfAmmo -= InNumberOfAmmo;
			AmountOfAmmoChanged(AmmoArray[i].CountOfAmmo);
			return true;
		}
	}
	return false;
}


void UKT_ItemsManagerComponent::Initialize(AKT_PlayerCharacter* InCharacter)
{
	PlayerCharacter = InCharacter;
}


void UKT_ItemsManagerComponent::ChangeWeapon()
{
	if (GetSelectedWeaponSlot() == FirstWeaponSlot && IsValid(SecondWeaponSlot))
	{
		SelectedFirstSlot = false;
		FirstWeaponSlot->CanShoot = false;
		GetWorld()->GetTimerManager().ClearAllTimersForObject(FirstWeaponSlot);
		SecondWeaponSlot->CanShoot = true;
	}
	else if (GetSelectedWeaponSlot() == SecondWeaponSlot && IsValid(FirstWeaponSlot))
	{
		SelectedFirstSlot = true;
		SecondWeaponSlot->CanShoot = false;
		GetWorld()->GetTimerManager().ClearAllTimersForObject(SecondWeaponSlot);
		FirstWeaponSlot->CanShoot = true;
	}
	ChangeIcon();
}
