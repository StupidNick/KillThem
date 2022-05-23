#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "KT_BaseGameState.generated.h"


class AKT_BaseGameMode;
UCLASS()
class KILLTHEM_API AKT_BaseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void GameTimerUpdate();

	UFUNCTION()
		virtual void TimeIsOver(){};
	
protected:
	
	FTimerHandle GameTimerHandle;
	int32 GameTimer = 0;

	
public:
	
	UPROPERTY()
		AKT_BaseGameMode* MyGameMode;
};
