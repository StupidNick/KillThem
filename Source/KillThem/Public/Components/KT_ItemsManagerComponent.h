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

//Protected C++ functions
protected:

	virtual void BeginPlay() override;

//Public C++ functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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
    	void AddAmmoOnServer(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmoFound);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | Weapon")
		void ChangeAmmoOnClient(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InAmmo);

	UFUNCTION()
		bool FindAndCountAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, int &InNumberOfAmmo);

	UFUNCTION(Server, Reliable)
		void RemoveAmmoOnServer(TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmo);

	UFUNCTION(Server, Reliable)
		void ChangeWeapon();

	UFUNCTION()
		void Initialize(AKT_PlayerCharacter* InCharacter);

	
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
	

	FORCEINLINE AKT_BaseWeapon*& GetSelectedWeaponSlot()
	{
		if (SelectedFirstSlot)
		{
			return FirstWeaponSlot;
		}
		return SecondWeaponSlot;
	}

	

//Public BP variables
public:

	UPROPERTY(BlueprintReadWrite, Category = "Character | Weapons")
		AKT_PlayerCharacter* PlayerCharacter = nullptr;

	
	UPROPERTY(Replicated)
		AKT_BaseWeapon* FirstWeaponSlot = nullptr;
	UPROPERTY(EditAnywhere, Category = "Character | Weapons")
		TSubclassOf<AKT_BaseWeapon> FirstWeaponSlotClass = nullptr;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Character | Weapons")
		AKT_BaseWeapon* SecondWeaponSlot = nullptr;

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

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponChanged, Category = "Status")
		bool SelectedFirstSlot = true;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName InHandsSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName BehindBackSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Grenade")
		FName FirstGrenadeSlotSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Grenade")
		FName SecondGrenadeSlotSocketName;

	UPROPERTY(EditAnywhere, Category = "Stats")
		TArray<FAmmo> AmmoArray;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnHandWeaponAmmoChange OnHandWeaponAmmoChangeBind;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnAmmoChange OnAmmoChangeBind;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnAmmoInTheClipChange OnAmmoInTheClipChange;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnWeaponChange OnWeaponChange;
};
