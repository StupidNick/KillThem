#pragma once

#include "CoreMinimal.h"
#include "Components/KT_CharacterMovementComponent.h"


#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameMode/KT_PlayerStart.h"


#include "KT_PlayerCharacter.generated.h"


class UImage;
class AKT_BaseGrenade;
class AKT_BaseInteractiveObject;
class UKT_ItemsManagerComponent;
class UBoxComponent;
class UCameraComponent;
class UMovementComponent;
class UCurveFloat;
class UKT_HealthComponent;
class AKT_BaseWeapon;
class AKT_GameHUD;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBusterActivated, UTexture2D*, Icon, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBusterUpdates, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBusterDeactivated, bool, Deactivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDashRecoverUpdates, const float&, Value, const int32&, Counter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDashUsed, const int32&, Counter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, APawn*, Player);


UCLASS()
class KILLTHEM_API AKT_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

//Constructor
public:
	
	AKT_PlayerCharacter(const FObjectInitializer& ObjInit);
	

//private с++ functions
private:

	UFUNCTION()
		void Initialize();

	UFUNCTION()
		void ScopingTimeLineFloatReturn(float Value);

	UFUNCTION()
		FTransform CalculateADSTransform();

	FOnTimelineFloat ScopingInterpFunction{};

//private с++ variables
private:

	UPROPERTY()
		UTimelineComponent* ScopingTimeLine;

	UPROPERTY()
		FTransform DefaultArmsTransform;
	

//protected c++ functions
protected:
	
	virtual void BeginPlay() override;

/////////////////////////////////////Weapon////////////////////////////////////////

	// UFUNCTION(Server, Reliable)
	// 	void DropFirstGrenadeOnServer();
	//
	// UFUNCTION(Server, Reliable)
	// 	void DropSecondGrenadeOnServer();

	UFUNCTION()
		void RightClick();

	UFUNCTION(Server, Reliable)
		void RightUnClick();

	UFUNCTION(Client, Reliable)
		void Scope();
	UFUNCTION(Server, Reliable)
		void ScopeOnServer();

	UFUNCTION(Client, Reliable)
		void UnScope();
	UFUNCTION(Server, Reliable)
		void UnScopeOnServer();

/////////////////////////////////////Interact//////////////////////////////////////

	UFUNCTION()
		void Interact();

	UFUNCTION(Server, Reliable)
		void CallInteractOnServer();

	UFUNCTION()
		void Destruction();
	
	
//protected c++ variables
protected:

	UPROPERTY(Replicated)
		bool CanInteract;

	UPROPERTY(Replicated)
		AKT_BaseInteractiveObject* InteractiveObject = nullptr;
	

//public c++ functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/////////////////////////////////////Boosters//////////////////////////////////////
	
	UFUNCTION(Server, Reliable)
		void SetSpeedBoost(const float& Boost);

	UFUNCTION(Server, Reliable)
		void SetBerserkBoost(const float& Boost);

	UFUNCTION(Server, Reliable)
		void SetRageBoost(const float& Boost);

	// UFUNCTION(Server, Reliable)
 //    	void AddGrenade(TSubclassOf<AKT_BaseGrenade> InGrenadeClass, const bool InToFirstSlot);

/////////////////////////////////////Interact//////////////////////////////////////
	
	UFUNCTION(Server, Reliable)
		void InteractInfoOnServer(AKT_BaseInteractiveObject* InInteractiveObject);

	UFUNCTION(Server, Unreliable)
		void UnInteractInfo();

/////////////////////////////////////Die/////////////////////////////////////////

	UFUNCTION()
		void Die(AController* Player);

	UFUNCTION(NetMulticast, Reliable)
		void DieMulticast();

	UFUNCTION(Client, Reliable)
		void DieOnClient();

	
//public c++ variables
public:

/////////////////////////////////Boosters/////////////////////////////////////////////

	UPROPERTY(Replicated)
		float DamageBooster = 1.0f;

	UPROPERTY(Replicated)
		float BerserkBooster = 1.0f;

	UPROPERTY(Replicated)
		bool IsScoping = false;

//public BP variables
public:

//////////////////////////////////////////////////////Components////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category = "Character | Components")
		UKT_ItemsManagerComponent* ItemsManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Components")
		UKT_HealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Components")
		UKT_CharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Components")
		USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		UCapsuleComponent* ParkourCapsuleComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		UBoxComponent* WallRunRightCollisionComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		UBoxComponent* WallRunLeftCollisionComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		APlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Character | Components")
		AKT_GameHUD* HUD;

/////////////////////////////////////More/////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Character | Weapons")
		UCurveFloat* ScopingCameraTilt;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FOnBusterActivated OnBoosterActivated;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FOnBusterUpdates OnBoosterUpdates;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FDashRecoverUpdates OnDashRecoverUpdates;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FDashUsed OnDashUsed;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FOnBusterDeactivated OnBoosterDeactivated;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FOnDead OnDead;
};