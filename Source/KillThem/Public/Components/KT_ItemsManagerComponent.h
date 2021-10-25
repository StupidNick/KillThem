#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/KT_BaseWeapon.h"
#include "Weapons/Grenades/KT_BaseGrenade.h"

#include "KT_ItemsManagerComponent.generated.h"


class AKT_PlayerCharacter;



USTRUCT()
struct FAmmo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TSubclassOf<AKT_BaseWeapon> TypeOfAmmo;

	int CountOfAmmo;
	
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

	UFUNCTION()
    	void AddAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, const int InNumberOfAmmoFound);

	UFUNCTION()
		bool FindAndCountAmmo(const TSubclassOf<AKT_BaseWeapon> InAmmoClass, int &InNumberOfAmmo);

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

	
	UPROPERTY(BlueprintReadWrite, Category = "Character | Weapons")
		AKT_BaseWeapon* FirstWeaponSlot = nullptr;
	UPROPERTY(EditAnywhere, Category = "Character | Weapons")
		TSubclassOf<AKT_BaseWeapon> FirstWeaponSlotClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "Character | Weapons")
		AKT_BaseWeapon* SecondWeaponSlot = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		AKT_BaseGrenade* FirstGrenadeSlot = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		AKT_BaseGrenade* SecondGrenadeSlot = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		TSubclassOf<AKT_BaseGrenade> FirstGrenadeSlotClass;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Grenade")
		TSubclassOf<AKT_BaseGrenade> SecondGrenadeSlotClass;

	UPROPERTY(BlueprintReadWrite, Category = "Status")
		bool SelectedFirstSlot = true;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName InHandsSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName BehindBackSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Grenade")
		FName FirstGrenadeSlotSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Character | Grenade")
		FName SecondGrenadeSlotSocketName;

	UPROPERTY(EditAnywhere, Category = "WeaponsAmmo")
		TArray<FAmmo> AmmoArray;
};
