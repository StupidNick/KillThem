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
