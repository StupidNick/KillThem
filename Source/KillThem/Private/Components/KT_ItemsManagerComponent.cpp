#include "Components/KT_ItemsManagerComponent.h"

#include "Character/KT_PlayerCharacter.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"


UKT_ItemsManagerComponent::UKT_ItemsManagerComponent()
{
}


void UKT_ItemsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	
}





void UKT_ItemsManagerComponent::Initialize(AKT_PlayerCharacter* InCharacter)
{
	PlayerCharacter = InCharacter;
}
