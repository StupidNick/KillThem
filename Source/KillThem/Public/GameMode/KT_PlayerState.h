#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KT_PlayerState.generated.h"



UCLASS()
class KILLTHEM_API AKT_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	FORCEINLINE void SetTeamID(const int32& InTeamID)
	{
		this->TeamID = InTeamID;
	}

	FORCEINLINE void SetTeamColor(const FLinearColor& InTeamColor)
	{
		this->TeamColor = InTeamColor;
	}

	FORCEINLINE int32 GetTeamID() const
	{
		return TeamID;
	}

	FORCEINLINE FLinearColor GetTeamColor() const
	{
		return TeamColor;
	}
	
	
//private C++ variables
private:

	int32 TeamID;
	FLinearColor TeamColor;

};
