#pragma once

#include "CoreMinimal.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "KT_TDMPlayerController.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTDMStatisticTableUpdate, const TArray<AKT_PlayerState*>&, TeammatesPlayerStates, const TArray<AKT_PlayerState*>&, EnemiesPlayerStates);



UCLASS()
class KILLTHEM_API AKT_TDMPlayerController : public AKT_PlayerController
{
	GENERATED_BODY()

//protected c++ functions
protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupInputComponent() override;
	
	UFUNCTION(Server, Reliable)
		void CollectPlayersStates();

	UFUNCTION(Client, Reliable)
		void ShowStatistic(const TArray<AKT_PlayerState*>& TeammatesPlayerStates, const TArray<AKT_PlayerState*>& EnemiesPlayerStates);

	virtual void HideStatistic() override;


//public c++ functions
public:

	UFUNCTION(NetMulticast, Reliable)
		void UpdateStat(const TArray<AKT_PlayerState*>& InArrayOfTeammatesPlayerState, const TArray<AKT_PlayerState*>& InArrayOfEnemiesPlayerState);
	

//Public BP variables	
public:
	
	UPROPERTY(BlueprintReadOnly)
		class AKT_TeamDeathmatchGameMode* CurrentGameMode;

	UPROPERTY(BlueprintReadOnly, Replicated)
		TArray<AKT_PlayerState*> ArrayOfTeammatesPlayerState;

	UPROPERTY(BlueprintReadOnly, Replicated)
		TArray<AKT_PlayerState*> ArrayOfEnemiesPlayerState;

	UPROPERTY(BlueprintAssignable)
		FTDMStatisticTableUpdate TDMStatisticTableUpdate;
};
