#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KT_HealthComponent.generated.h"


class AKT_PlayerCharacter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHPChange, float, HPStat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSPChange, float, SPStat);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UKT_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

//Constructors
public:	

	UKT_HealthComponent();

//private C++ functions
private:


//private C++ variables
private:

	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_Health, Category = "Stats")
		float Health;

	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_Shield, Category = "Stats")
		float Shield;

	bool IsDead = false;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, const float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	
//public BP functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/////////////////////////////////////Setters/////////////////////////////////////////
	


/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////Getters/////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		FORCEINLINE float GetHealth() const{return Health;}

	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Shield")
		FORCEINLINE float GetShield() const{return Shield;}

	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		FORCEINLINE float GetIsDead() const{return IsDead;}

/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////Health//////////////////////////////////////////

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_Health();
	
	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		void ChangeHealth(const float InHealth);

	UFUNCTION(Server, Reliable)
		void ChangeHealthOnServer(const float InHealth);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		void Death();

	UFUNCTION(Server, Reliable)
		void DeathOnServer();

/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////Shield//////////////////////////////////////////

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_Shield();
	
	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		void ChangeShield(const float InShield);

	UFUNCTION(Server, Reliable)
		void ChangeShieldOnServer(const float InShield);

/////////////////////////////////////////////////////////////////////////////////////

	
//public C++ variables
public:

	UFUNCTION()
		void Initialize(AKT_PlayerCharacter* InCharacter);
	
//public BP variables
public:

	UPROPERTY(EditAnywhere, Category = "HealthComponent | Health")
		AKT_PlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FOnHPChange OnHPChangeBind;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent | EventsForBind")
		FOnSPChange OnSPChangeBind;

	UPROPERTY(EditAnywhere, Category = "HealthComponent | Health")
		float DefaultHealth;

	UPROPERTY(EditAnywhere, Category = "HealthComponent | Health")
		float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "HealthComponent | Shield")
		float DefaultShield;

	UPROPERTY(EditAnywhere, Category = "HealthComponent | Shield")
		float ShieldSafeFactor;
};
