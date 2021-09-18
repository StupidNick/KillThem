#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/KT_BaseWeapon.h"

#include "KT_ItemsManagerComponent.generated.h"


class AKT_PlayerCharacter;



UENUM()
enum SelectedWeaponSlot
{
	FirstSlot	UMETA(DisplayName = "FirstSlot"),
	SecondSlot	UMETA(DisplayName = "SecondSlot"),
	FirstGrenadeSlot	UMETA(DisplayName = "FirstGrenadeSlot"),
	SecondGrenadeSlot	UMETA(DisplayName = "SecondGrenadeSlot"),
};


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
	//
	// UFUNCTION()
	// 	void ChangeWeapon();

	UFUNCTION()
		void Initialize(AKT_PlayerCharacter* InCharacter);

	UFUNCTION()
		FORCEINLINE AKT_BaseWeapon*& GetSelectedWeaponSlot()
	{
		if (SelectedWeaponSlotEnum.GetValue() == FirstSlot)
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

	UPROPERTY(BlueprintReadWrite, Category = "Character | Weapons")
		AKT_BaseWeapon* FirstGrenadeSlot = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Character | Weapons")
		AKT_BaseWeapon* SecondGrenadeSlot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		TEnumAsByte<SelectedWeaponSlot> SelectedWeaponSlotEnum;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName InHandsSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Character | Weapons")
		FName BehindBackSocketName;

	UPROPERTY(EditAnywhere, Category = "WeaponsAmmo")
		TArray<FAmmo> AmmoArray;
};
