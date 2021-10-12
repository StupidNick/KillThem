#include "Components/KT_ItemsManagerComponent.h"

#include "Character/KT_PlayerCharacter.h"


UKT_ItemsManagerComponent::UKT_ItemsManagerComponent()
{
}


void UKT_ItemsManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UKT_ItemsManagerComponent::AddAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmoFound)
{
	for (int i = 0; i < AmmoArray.Num(); i++)
	{
		if (AmmoArray[i].TypeOfAmmo == InAmmoClass)
		{
			AmmoArray[i].CountOfAmmo += InNumberOfAmmoFound;
			UE_LOG(LogTemp, Error, TEXT("%s: %i"), *PlayerCharacter->GetName(), AmmoArray[i].CountOfAmmo);
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


bool UKT_ItemsManagerComponent::RemoveAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmo)
{
	for (int i = 0; i < AmmoArray.Num(); i++)
	{
		if (AmmoArray[i].TypeOfAmmo == InAmmoClass && AmmoArray[i].CountOfAmmo >= InNumberOfAmmo)
		{
			AmmoArray[i].CountOfAmmo -= InNumberOfAmmo;
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
	
}