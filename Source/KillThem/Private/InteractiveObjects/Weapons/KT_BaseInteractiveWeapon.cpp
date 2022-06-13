#include "InteractiveObjects/Weapons/KT_BaseInteractiveWeapon.h"

#include "Weapons/KT_BaseWeapon.h"
#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_ItemsManagerComponent.h"
#include "GameMode/KT_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AKT_BaseInteractiveWeapon::AKT_BaseInteractiveWeapon()
{
	bReplicates = true;
}


void AKT_BaseInteractiveWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AKT_BaseInteractiveWeapon, AmmoInTheClip);
}


void AKT_BaseInteractiveWeapon::Destruction_Implementation()
{
	Destroy();
}


void AKT_BaseInteractiveWeapon::InteractiveOnServer(AKT_PlayerCharacter* Player)
{
	Super::InteractiveOnServer(Player);
	
	Player->ItemsManagerComponent->AddWeaponOnServer(WeaponClass, 0, AmmoInTheClip);
	Destruction();
}


void AKT_BaseInteractiveWeapon::Initialize_Implementation(const int16& InAmountOfAmmo)
{
	AmmoInTheClip = InAmountOfAmmo;

	// TArray<AActor*> LOverlappingActors;
	// InteractSphereCollision->GetOverlappingActors(LOverlappingActors);
	//
	// for (auto i : LOverlappingActors)
	// {
	// 	if (AKT_PlayerCharacter* LPlayer = Cast<AKT_PlayerCharacter>(i))
	// 	{
	// 		LPlayer->InteractInfoOnServer(this);
	// 	}
	// }
	
	FTimerHandle LTimerHandle;
	FTimerDelegate LTimerDelegate;
	
	LTimerDelegate.BindUFunction(this, "Destruction");
	GetWorldTimerManager().SetTimer(LTimerHandle, LTimerDelegate, Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->ItemsDestructionTimer, false);
}
