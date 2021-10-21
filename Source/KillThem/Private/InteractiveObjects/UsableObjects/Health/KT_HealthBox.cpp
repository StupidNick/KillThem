#include "InteractiveObjects/UsableObjects/Health/KT_HealthBox.h"

#include "Components/KT_HealthComponent.h"


AKT_HealthBox::AKT_HealthBox()
{
}


void AKT_HealthBox::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_HealthBox::Interactive(AKT_PlayerCharacter* Player)
{
	Super::Interactive(Player);

	Player->HealthComponent->ChangeHealth(RecoveryHealth);
}
