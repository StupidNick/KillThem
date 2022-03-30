#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/KT_BaseWeapon.h"
#include "Engine/Texture2D.h"
#include "InteractiveObjects/Ammo/KT_BaseAmmo.h"
#include "Weapons/Grenades/KT_BaseGrenade.h"

#include "KT_ItemsManagerComponent.generated.h"



class AKT_PlayerCharacter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandWeaponAmmoChange, int, AmmoStat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, TSubclassOf<AKT_BaseWeapon>, AmmoClass, int, AmmoStat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoInTheClipChange, int, AmmoStat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChange, UTexture2D*, WeaponIcon, UTexture2D*, AimIcon);



USTRUCT()
struct FAmmo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TSubclassOf<AKT_BaseWeapon> TypeOfAmmo;

	UPROPERTY(EditAnywhere)
		int MaxAmmo;

	int CountOfAmmo;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AKT_BaseAmmo> AmmoActorClass;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KILLTHEM_API UKT_ItemsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

//Constructors
public:	

	UKT_ItemsManagerComponent();

//Private C++ variables
private:

	UFUNCTION(Server, Reliable)
		void PlayerDead(APawn* InPlayer);

//Private C++ variables
private:

	UPROPERTY(ReplicatedUsing = OnRep_WeaponChanged)
		TArray<AKT_BaseWeapon*> WeaponsArray;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponChanged)
		int32 CurrentWeaponIndex = 0;

	UPROPERTY(Replicated)
		bool CanShoot = true;

//Protected C++ functions
protected:

	virtual void BeginPlay() override;

//Public C++ functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

////////////////////////////////////////////////////Ammo////////////////////////////////////////////////////////////////
	
	UFUNCTION(NetMulticast, Reliable)
		void AmountOfAmmoHandWeaponChanged(const int InAmmo);

	UFUNCTION(NetMulticast, Reliable)
		void AmountOfAmmoChanged(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InAmmo);

	UFUNCTION(NetMulticast, Reliable)
		void ChangeAmmoInTheClip(const int InAmmo);

	UFUNCTION(NetMulticast, Reliable)
		void ChangeIcon();

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_WeaponChanged();

	UFUNCTION(Server, Reliable)
    	void AddAmmoOnServer(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int16& InNumberOfAmmoFound);

	UFUNCTION(Server, Reliable)
    	void DropAmmoOnServer(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int16& InNumberOfAmmo, const FTransform& InAmmoTransform);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | Weapon")
		void ChangeAmmoOnClient(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InAmmo);

	UFUNCTION()
		bool CountAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, int &InNumberOfAmmo);

	UFUNCTION(Server, Reliable)
		void RemoveAmmoOnServer(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmo);

	FORCEINLINE FAmmo* FindStructOfAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass)
	{
		for (int i = 0; i < AmmoArray.Num(); i++)
		{
			if (AmmoArray[i].TypeOfAmmo == InAmmoClass)
			{
				return& AmmoArray[i];
			}
		}
		return nullptr;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////Weapon////////////////////////////////////////////////////////////////
	
	UFUNCTION(Server, Reliable)
		void AddWeapon(TSubclassOf<AKT_BaseWeapon> InWeaponClass, const int16& InAmountOfAmmo, const int16& InAmmoInTheClip = -1);
	
	UFUNCTION(Server, Reliable)
		void ChangeWeapon();

	UFUNCTION(Server, Reliable)
		void Reload();

	UFUNCTION(Server, Reliable)
		void SetCanShoot(const bool InCanShoot);

	UFUNCTION(Server, Reliable)
		void StartFire();

	UFUNCTION(Server, Reliable)
		void StopFire();

	UFUNCTION(Server, Reliable)
		void AttachWeaponToSocket(AKT_BaseWeapon* InWeapon, USceneComponent* InSceneComponent, const FName& InSocketName);

	UFUNCTION(Server, Reliable)
		void DetachWeaponFromActor(AKT_BaseWeapon* InWeapon);

	FORCEINLINE AKT_BaseWeapon*& GetSelectedWeaponSlot()
	{
		return WeaponsArray[CurrentWeaponIndex];
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Public C++ variables
public:

	UPROPERTY()
		bool WantShoot = true;

//Public BP variables
public:

	UPROPERTY(BlueprintReadWrite, Category = "Character | Weapons")
		AKT_PlayerCharacter* PlayerCharacter = nullptr;

/////////////////////////////////Weapons///////////////////////////////////////////////////
	
	UPROPERTY(EditAnywhere, Category = "Character | Weapons")
		TSubclassOf<AKT_BaseWeapon> FirstWeaponSlotClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Character | Weapons")
		int AmmoForFirstWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		AKT_BaseGrenade* FirstGrenadeSlot = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		AKT_BaseGrenade* SecondGrenadeSlot = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		TSubclassOf<AKT_BaseGrenade> FirstGrenadeSlotClass;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		TSubclassOf<AKT_BaseGrenade> SecondGrenadeSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName HandsSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName BehindBackSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Grenade")
		FName FirstGrenadeSlotSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Grenade")
		FName SecondGrenadeSlotSocketName;

////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////Ammo///////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Stats")
		TArray<FAmmo> AmmoArray;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		int8 PercentOfDroppedAmmo;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnHandWeaponAmmoChange OnHandWeaponAmmoChangeBind;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnAmmoChange OnAmmoChangeBind;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnAmmoInTheClipChange OnAmmoInTheClipChange;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnWeaponChange OnWeaponChange;
};
