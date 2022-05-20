#pragma once

#include "CoreMinimal.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "KT_DeathmatchPlayerController.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMStatisticTableUpdate, const TArray<AKT_PlayerState*>&, PlayerStatesArray);



UCLASS()
class KILLTHEM_API AKT_DeathmatchPlayerController : public AKT_PlayerController
{
	GENERATED_BODY()

//protected c++ functions
protected:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable)
		void CollectPlayersStates();

	UFUNCTION(Client, Reliable)
		void ShowStatistic(const TArray<AKT_PlayerState*>& PlayerStatesArray);

	virtual void HideStatistic() override;

//Public C++ functions	
public:
	
	UFUNCTION(NetMulticast, Reliable)
		void UpdateStat(const TArray<AKT_PlayerState*>& PlayerStatesArray);

	
//Public BP variables	
public:
	
	UPROPERTY(BlueprintReadOnly)
		class AKT_DeathmatchGameMode* CurrentGameMode;

	UPROPERTY(BlueprintAssignable)
		FDMStatisticTableUpdate DMStatisticTableUpdate;
};
