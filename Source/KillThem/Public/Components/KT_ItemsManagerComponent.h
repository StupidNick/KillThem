#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KT_ItemsManagerComponent.generated.h"


class AKT_PlayerCharacter;
class AKT_BaseWeapon;







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

	// UFUNCTION()
 //    	void AddWeapon(TSubclassOf<AKT_BaseWeapon> InWeaponClass);
	//
	// UFUNCTION()
	// 	void ChangeWeapon();

	UFUNCTION()
		void Initialize(AKT_PlayerCharacter* InCharacter);

	

//Public BP variables
public:

	UPROPERTY(BlueprintReadWrite, Category = "Character | Weapons")
		AKT_PlayerCharacter* PlayerCharacter = nullptr;
	
	
};
