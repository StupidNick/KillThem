#pragma once

#include "CoreMinimal.h"
#include "KT_CoreTypes.h"
#include "Character/KT_SpectatorPawn.h"
#include "GameFramework/PlayerController.h"
#include "KT_PlayerController.generated.h"


class UKT_StatisticsLineWD;
class AKT_PlayerState;
class AKT_PlayerCharacter;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerOfDeath, const int32&, Timer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRespawnReady, const bool, CanRespawn);



UCLASS()
class KILLTHEM_API AKT_PlayerController : public APlayerController
{
	GENERATED_BODY()


//private c++ variables
private:

	FTimerHandle RespawnTimerHandle;
	FGameData GameData;

	UPROPERTY()
		class AKT_BaseGameMode* GameMode;
	
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

	UFUNCTION(Client, Reliable)
		virtual void HideStatistic();

	
//Blueprint Values
public:

	UPROPERTY()
		AKT_PlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class AKT_SpectatorPawn* SpectatorPlayerPawn;

	UPROPERTY(BlueprintReadOnly)
		class AKT_GameHUD* GameHUD;

	UPROPERTY(BlueprintAssignable)
		FTimerOfDeath TimerOfDeath;

	UPROPERTY(BlueprintAssignable)
		FRespawnReady RespawnReady;
};
