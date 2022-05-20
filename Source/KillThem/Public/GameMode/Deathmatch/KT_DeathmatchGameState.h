#pragma once

#include "CoreMinimal.h"
#include "GameMode/KT_BaseGameState.h"
#include "KT_DeathmatchGameState.generated.h"




UCLASS()
class KILLTHEM_API AKT_DeathmatchGameState : public AKT_BaseGameState
{
	GENERATED_BODY()

public:	

	virtual void TimeIsOver() override;
};
