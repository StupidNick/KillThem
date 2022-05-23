#pragma once

#include "CoreMinimal.h"
#include "KT_DeathmatchGameState.h"
#include "GameMode/KT_BaseGameMode.h"
#include "KT_DeathmatchGameMode.generated.h"



UCLASS()
class KILLTHEM_API AKT_DeathmatchGameMode : public AKT_BaseGameMode
{
	GENERATED_BODY()


//public C++ functions
public:

	virtual void BeginPlay() override;

	virtual void Killed(const AController* KilledController, const AController* VictimController) override;

	virtual void UpdateStatistic() override;

	virtual void GameOver(const FString& WinnerName) override;


public:

	UPROPERTY()
		AKT_DeathmatchGameState* CurrentGameState;
};