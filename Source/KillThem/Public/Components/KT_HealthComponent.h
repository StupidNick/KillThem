#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KT_HealthComponent.generated.h"





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

	float Health;

	bool IsDead = false;

protected:

	virtual void BeginPlay() override;

	
//public BP functions
public:

/////////////////////////////////////Setters/////////////////////////////////////////
	


/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////Getters/////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		bool GetIsDead() const;

/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////Health//////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		void HealthRecovery(const float InHealth);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		void DecreaseInHealth(const float InHealth);

	UFUNCTION(Server, Reliable)
		void ChangeHealthOnServer(const bool Add, const float InHealth);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent | Health")
		void Death();

	UFUNCTION(Server, Reliable)
		void DeathOnServer();

/////////////////////////////////////////////////////////////////////////////////////


//public C++ variables
public:

	
//public BP variables
public:

	UPROPERTY(EditAnywhere, Category = "HealthComponent | Health")
		float DefaultHealth;

	UPROPERTY(EditAnywhere, Category = "HealthComponent | Health")
		float MaxHealth;
};
