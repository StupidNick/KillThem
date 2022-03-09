#include "InteractiveObjects/UsableObjects/Health/KT_HealthBox.h"

#include "Components/KT_HealthComponent.h"


AKT_HealthBox::AKT_HealthBox()
{
}


void AKT_HealthBox::BeginPlay()
{
	Super::BeginPlay();
}


void AKT_HealthBox::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	Player->HealthComponent->ChangeHealth(RecoveryHealth);
}
