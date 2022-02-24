#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/KT_BaseWeapon.h"
#include "Engine/Texture2D.h"
#include "InteractiveObjects/Ammo/KT_BaseAmmo.h"
#include "Weapons/Grenades/KT_BaseGrenade.h"

#include "KT_ItemsManagerComponent.generated.h"



class AKT_PlayerCharacter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChange, int, AmmoStat);
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
		void AmountOfAmmoChanged(const int Ammo);

	UFUNCTION(NetMulticast, Reliable)
		void ChangeAmmoInTheClip(int Ammo);

	UFUNCTION(NetMulticast, Reliable)
		void ChangeIcon();

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_WeaponChanged();

	UFUNCTION()
    	void AddAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmoFound);

	UFUNCTION()
		bool FindAndCountAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, int &InNumberOfAmmo);

	UFUNCTION()
		FAmmo FindStructOfAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass);

	UFUNCTION()
		bool RemoveAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmo);

	UFUNCTION()
		void ChangeWeapon();

	UFUNCTION()
		void Initialize(AKT_PlayerCharacter* InCharacter);

	UFUNCTION()
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

	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponChanged)
		AKT_BaseWeapon* FirstWeaponSlot = nullptr;
	UPROPERTY(EditAnywhere, Category = "Character | Weapons")
		TSubclassOf<AKT_BaseWeapon> FirstWeaponSlotClass = nullptr;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponChanged, Category = "Character | Weapons")
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
		FOnAmmoChange OnAmmoChangeBind;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnAmmoInTheClipChange OnAmmoInTheClipChange;

	UPROPERTY(BlueprintAssignable, Category = "ItemsManager | EventsForBind")
		FOnWeaponChange OnWeaponChange;
};
