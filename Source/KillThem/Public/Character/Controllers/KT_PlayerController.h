#pragma once

#include "CoreMinimal.h"
#include "KT_CoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "KT_PlayerController.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerOfDeath, const int32&, Timer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRespawnReady, const bool, CanRespawn);



UCLASS()
class KILLTHEM_API AKT_PlayerController : public APlayerController
{
	GENERATED_BODY()

	AKT_PlayerController();


//private c++ variables
private:

	FTimerHandle RespawnTimerHandle;
	FGameData GameData;
	
	UPROPERTY(ReplicatedUsing = OnRep_DeathTimerChanged)
		int32 DeathTimer;

	UPROPERTY(ReplicatedUsing = OnRep_ReadyToRespawn)
		bool ReadyToRespawn = false;

//protected c++ functions
protected:
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(NetMulticast, Reliable)
		virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client, Reliable)
		void OnEscapeButtonPressed();

	UFUNCTION(Server, Reliable)
		void CountDownToRespawn();

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_DeathTimerChanged();
	UFUNCTION(NetMulticast, Reliable)
		void OnRep_ReadyToRespawn();

//public c++ functions
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
		void PreparePlayerForRespawnOnServer();

	UFUNCTION(Client, Reliable)
		void PreparePlayerForRespawnOnClient();

	UFUNCTION()
		void PlayerInitialize();

	UFUNCTION(Client, Reliable)
		void RespawnPlayerOnClient();

	UFUNCTION(Server, Reliable)
		void RespawnPlayerOnServer();

	
//Blueprint Values
public:

	UPROPERTY(BlueprintReadOnly)
		class AKT_PlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class AKT_GameHUD* GameHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "20"))
		int32 RespawnTime;  // in seconds

	UPROPERTY(BlueprintAssignable)
		FTimerOfDeath TimerOfDeath;

	UPROPERTY(BlueprintAssignable)
		FRespawnReady RespawnReady;
};
