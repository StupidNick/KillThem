#include "InteractiveObjects/UsableObjects/Shield/KT_ShieldBox.h"

#include "Components/KT_HealthComponent.h"


AKT_ShieldBox::AKT_ShieldBox()
{
}


void AKT_ShieldBox::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_ShieldBox::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);

	Player->HealthComponent->ChangeShield(ShieldValue);
}
