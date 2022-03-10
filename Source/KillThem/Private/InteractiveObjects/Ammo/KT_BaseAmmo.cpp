#include "InteractiveObjects/Ammo/KT_BaseAmmo.h"

#include "Components/KT_ItemsManagerComponent.h"
#include "GameMode/KT_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"


AKT_BaseAmmo::AKT_BaseAmmo()
{
}


void AKT_BaseAmmo::Destruction_Implementation()
{
	Destroy();
}


void AKT_BaseAmmo::Initialize(int16 InAmountAmmo, bool IsDropped)
{
	CountOfAmmo = InAmountAmmo;
	IsDroppedAmmo = IsDropped;

	if (!IsDropped) return;
	
	FTimerHandle LTimerHandle;
	FTimerDelegate LTimerDelegate;
	
	LTimerDelegate.BindUFunction(this, "Destruction");
	GetWorldTimerManager().SetTimer(LTimerHandle, LTimerDelegate, Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(this))->ItemsDestructionTimer, false);
}


void AKT_BaseAmmo::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);

	Player->ItemsManagerComponent->AddAmmoOnServer(ClassOfAmmo, CountOfAmmo);

	if (IsDroppedAmmo)
	{
		Destroy();
	}
	else
	{
		DisableObject();
		if (RecoverTime > 0)
		{
			EnableTimerDelegate.BindUFunction(this, "ToEnableObject", Player);
			GetWorldTimerManager().SetTimer(EnableTimerHandle, EnableTimerDelegate, RecoverTime, false);
		}
	}
}
