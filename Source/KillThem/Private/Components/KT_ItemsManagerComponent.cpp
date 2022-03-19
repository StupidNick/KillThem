#include "Components/KT_ItemsManagerComponent.h"

#include "Character/KT_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/RangeWeapon/KT_BaseRangeWeapon.h"



/////////////////////////////////////////////Initialize//////////////////////////////////////////////

UKT_ItemsManagerComponent::UKT_ItemsManagerComponent()
{
}


void UKT_ItemsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAmmoChangeBind.AddDynamic(this, &UKT_ItemsManagerComponent::ChangeAmmoOnClient);

	PlayerCharacter = Cast<AKT_PlayerCharacter>(GetOwner());
	if (!IsValid(FirstWeaponSlotClass) || !IsValid(PlayerCharacter)) return;

	if (PlayerCharacter->HasAuthority())
	{
		AddWeapon(FirstWeaponSlotClass, AmmoForFirstWeapon);
		PlayerCharacter->OnDead.AddDynamic(this, &UKT_ItemsManagerComponent::PlayerDead);
	}
}


void UKT_ItemsManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKT_ItemsManagerComponent, WeaponsArray);
	DOREPLIFETIME(UKT_ItemsManagerComponent, CurrentWeaponIndex);
}


void UKT_ItemsManagerComponent::PlayerDead_Implementation(APawn* InPlayer)
{
	int LAmmo;
	
	if (CountAmmo(GetSelectedWeaponSlot()->GetClass(), LAmmo))
	{
		DropAmmoOnServer(GetSelectedWeaponSlot()->GetClass(), LAmmo / PercentOfDroppedAmmo, PlayerCharacter->GetTransform());
	}
	DetachWeaponFromActor(GetSelectedWeaponSlot());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////Ammo//////////////////////////////////////////////

void UKT_ItemsManagerComponent::AmountOfAmmoChanged_Implementation(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InAmmo)
{
	OnAmmoChangeBind.Broadcast(InAmmoClass, InAmmo);
}


void UKT_ItemsManagerComponent::ChangeAmmoOnClient_Implementation(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InAmmo)
{
	if (FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		LAmmoStruct->CountOfAmmo = InAmmo;
	}
}


void UKT_ItemsManagerComponent::ChangeIcon_Implementation()
{
	OnWeaponChange.Broadcast(GetSelectedWeaponSlot()->WeaponIcon, GetSelectedWeaponSlot()->AimIcon);
}


void UKT_ItemsManagerComponent::ChangeAmmoInTheClip_Implementation(const int InAmmo)
{
	OnAmmoInTheClipChange.Broadcast(InAmmo);
}


void UKT_ItemsManagerComponent::AmountOfAmmoHandWeaponChanged_Implementation(const int InAmmo)
{
	OnHandWeaponAmmoChangeBind.Broadcast(InAmmo);
}


void UKT_ItemsManagerComponent::OnRep_WeaponChanged_Implementation()
{
	if (IsValid(GetSelectedWeaponSlot()))
	{
		int LAmountAmmo;
		OnAmmoInTheClipChange.Broadcast(GetSelectedWeaponSlot()->GetAmmoInTheClip());
		if (CountAmmo(GetSelectedWeaponSlot()->GetClass(), LAmountAmmo))
		{
			OnHandWeaponAmmoChangeBind.Broadcast(LAmountAmmo);
		}
		OnWeaponChange.Broadcast(GetSelectedWeaponSlot()->WeaponIcon, GetSelectedWeaponSlot()->AimIcon);
	}
}


void UKT_ItemsManagerComponent::AddAmmoOnServer_Implementation(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int16& InNumberOfAmmoFound)
{
	if (FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		LAmmoStruct->CountOfAmmo += InNumberOfAmmoFound;
		if (LAmmoStruct->CountOfAmmo > LAmmoStruct->MaxAmmo)
		{
			LAmmoStruct->CountOfAmmo = LAmmoStruct->MaxAmmo;
		}
		if (GetSelectedWeaponSlot()->GetClass() == InAmmoClass)
		{
			AmountOfAmmoHandWeaponChanged(LAmmoStruct->CountOfAmmo);
			AmountOfAmmoChanged(InAmmoClass, LAmmoStruct->CountOfAmmo);
		}
		else if(GetSelectedWeaponSlot()->GetClass() != LAmmoStruct->TypeOfAmmo)
		{
			AmountOfAmmoChanged(InAmmoClass, LAmmoStruct->CountOfAmmo);
		}
	}
}


void UKT_ItemsManagerComponent::DropAmmoOnServer_Implementation(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int16& InNumberOfAmmo, const FTransform& InAmmoTransform)
{
	if (const FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		AKT_BaseAmmo* LDroppedAmmo = GetWorld()->SpawnActor<AKT_BaseAmmo>(LAmmoStruct->AmmoActorClass, InAmmoTransform);
		if (IsValid(LDroppedAmmo))
		{
			LDroppedAmmo->Initialize(InNumberOfAmmo, true);
		}
	}
}


bool UKT_ItemsManagerComponent::CountAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass,
                                          int& InNumberOfAmmo)
{
	if (const FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		InNumberOfAmmo = LAmmoStruct->CountOfAmmo;
		if (InNumberOfAmmo > 0)
		{
			return true;
		}
	}
	return false;
}


void UKT_ItemsManagerComponent::RemoveAmmoOnServer_Implementation(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmo)
{
	if (FAmmo* LAmmoStruct = FindStructOfAmmo(InAmmoClass))
	{
		if (LAmmoStruct->CountOfAmmo >= InNumberOfAmmo)
		{
			LAmmoStruct->CountOfAmmo -= InNumberOfAmmo;
			AmountOfAmmoHandWeaponChanged(LAmmoStruct->CountOfAmmo);
			AmountOfAmmoChanged(InAmmoClass, LAmmoStruct->CountOfAmmo);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////Weapon//////////////////////////////////////////////

void UKT_ItemsManagerComponent::AddWeapon_Implementation(TSubclassOf<AKT_BaseWeapon> InWeaponClass, const int16& InAmountOfAmmo, const int16& InAmmoInTheClip)
{
	if (WeaponsArray.Num() == 0)
	{
		const auto LWeapon = GetWorld()->SpawnActor<AKT_BaseWeapon>(InWeaponClass);
		if (IsValid(LWeapon))
		{
			LWeapon->SetOwner(PlayerCharacter);
			LWeapon->Initialize(PlayerCharacter, InAmmoInTheClip);
			WeaponsArray.Add(LWeapon);
			AddAmmoOnServer(InWeaponClass, InAmountOfAmmo);

			AttachWeaponToSocket(LWeapon, PlayerCharacter->GetMesh(), HandsSocketName);
		}
		return;
	}
	if (WeaponsArray.Num() == 1)
	{
		const auto LWeapon = GetWorld()->SpawnActor<AKT_BaseWeapon>(InWeaponClass);
		if (IsValid(LWeapon))
		{
			LWeapon->SetOwner(PlayerCharacter);
			LWeapon->Initialize(PlayerCharacter, InAmmoInTheClip);
			WeaponsArray.Add(LWeapon);
			AddAmmoOnServer(InWeaponClass, InAmountOfAmmo);
			
			AttachWeaponToSocket(LWeapon, PlayerCharacter->GetMesh(), BehindBackSocketName);
		}
		return;
	}
	DetachWeaponFromActor(GetSelectedWeaponSlot());
	GetSelectedWeaponSlot() = GetWorld()->SpawnActor<AKT_BaseWeapon>(InWeaponClass);
	if (IsValid(GetSelectedWeaponSlot()))
	{
		GetSelectedWeaponSlot()->SetOwner(PlayerCharacter);
		GetSelectedWeaponSlot()->Initialize(PlayerCharacter, InAmmoInTheClip);
		AddAmmoOnServer(InWeaponClass, InAmountOfAmmo);
			
		AttachWeaponToSocket(GetSelectedWeaponSlot(), PlayerCharacter->GetMesh(), HandsSocketName);
	}
}


void UKT_ItemsManagerComponent::DetachWeaponFromActor_Implementation(AKT_BaseWeapon* InWeapon)
{
	if (!IsValid(InWeapon) || !IsValid(PlayerCharacter)) return;
	
	InWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	const FVector LLocation = InWeapon->GetActorLocation() + FVector(100, 0, 0);
	const FRotator LRotation = InWeapon->GetActorRotation();
	const FActorSpawnParameters LSpawnInfo;

	if (PlayerCharacter->HasAuthority())
	{
		AKT_BaseInteractiveWeapon* LDroppedWeapon = GetWorld()->SpawnActor<AKT_BaseInteractiveWeapon>(InWeapon->InteractiveWeaponClass, LLocation, LRotation, LSpawnInfo);
		if (IsValid(LDroppedWeapon))
		{
			LDroppedWeapon->Initialize(InWeapon->GetAmmoInTheClip());
			LDroppedWeapon->StaticMesh->SetCollisionProfileName(FName("BlockAll"));
			LDroppedWeapon->StaticMesh->SetSimulatePhysics(true);
		}
	}
	InWeapon->Destroy();
}


void UKT_ItemsManagerComponent::AttachWeaponToSocket_Implementation(AKT_BaseWeapon* InWeapon, USceneComponent* InSceneComponent,
                                                                    const FName& InSocketName)
{
	if (!IsValid(InWeapon) || !IsValid(InSceneComponent)) return;

	const FAttachmentTransformRules LAttachmentRules(EAttachmentRule::SnapToTarget, false);
	InWeapon->AttachToComponent(InSceneComponent, LAttachmentRules, InSocketName);
}


void UKT_ItemsManagerComponent::ChangeWeapon_Implementation()
{
	if (!PlayerCharacter) return;
	
	if (IsValid(GetSelectedWeaponSlot()))
	{
		AttachWeaponToSocket(GetSelectedWeaponSlot(), PlayerCharacter->GetMesh(), BehindBackSocketName);
		GetSelectedWeaponSlot()->BreakAllAction();
	}
	
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponsArray.Num();

	if (IsValid(GetSelectedWeaponSlot()))
	{
		AttachWeaponToSocket(GetSelectedWeaponSlot(), PlayerCharacter->GetMesh(), HandsSocketName);
	}
}


void UKT_ItemsManagerComponent::Reload_Implementation()
{
	if (!IsValid(PlayerCharacter)) return;
	
	if (PlayerCharacter->IsSprinted)
	{
		PlayerCharacter->BreakSprint();
	}
	Cast<AKT_BaseRangeWeapon>(GetSelectedWeaponSlot())->ToReload();
}


void UKT_ItemsManagerComponent::StartFire_Implementation()
{
	if (!IsValid(PlayerCharacter)) return;

	WantShoot = true;
	if (IsValid(GetSelectedWeaponSlot()))
	{
		GetSelectedWeaponSlot()->UseWeapon();
	}
}


void UKT_ItemsManagerComponent::StopFire_Implementation()
{
	WantShoot = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////