#pragma once

#include "CoreMinimal.h"
#include "GameMode/KT_BaseGameState.h"
#include "KT_TeamDeathmatchGameState.generated.h"


UCLASS()
class KILLTHEM_API AKT_TeamDeathmatchGameState : public AKT_BaseGameState
{
	GENERATED_BODY()

public:

	void IncrementTeamScore(const int32& TeamID);
	
	void DecrementTeamScore(const int32& TeamID);

	virtual void TimeIsOver() override;

protected:

	virtual void BeginPlay() override;

	
protected:

	int32 FirstTeamScore;
	int32 SecondTeamScore;
};
