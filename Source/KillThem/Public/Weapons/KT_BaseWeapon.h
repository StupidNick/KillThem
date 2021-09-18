#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Actor.h"
#include "KT_BaseWeapon.generated.h"


class AKT_PlayerCharacter;
UCLASS()
class KILLTHEM_API AKT_BaseWeapon : public AActor
{
	GENERATED_BODY()

//Constructors
public:	

	AKT_BaseWeapon();

//private C++ functions
private:

//private C++ variables
private:

	
//private C++ functions
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void UseWeapon();

	UFUNCTION()
		void AutoFireReload();

//private C++ variables
protected:

	FTimerHandle AutoFireTimerHandle;
	
	FTimerDelegate AutoFireTimerDelegate;

//public C++ variables
public:

	UFUNCTION()
		void ToUseWeapon();

	UFUNCTION(BlueprintNativeEvent)
		void Initialize(AKT_PlayerCharacter* InCharacter);
		virtual void Initialize_Implementation(AKT_PlayerCharacter* InCharacter);

	UFUNCTION()
		void ToAttachToComponent(USkeletalMeshComponent*& InComponent, const FName InSocketName);

	UFUNCTION()
		void ToDetachFromActor();

//public C++ variables
public:

	bool CanShoot = true;
	
//public BP variables
public:

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		AKT_PlayerCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float DelayBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool AutoFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Damage;
};
