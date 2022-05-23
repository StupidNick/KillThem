#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KT_PlayerState.generated.h"


class AKT_BaseGameMode;
UCLASS()
class KILLTHEM_API AKT_PlayerState : public APlayerState
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
		void OnRep_OnKillsNumChanged();
	
	UFUNCTION(NetMulticast, Reliable)
		void OnRep_OnDeathsNumChanged();
	
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
	
	FORCEINLINE int32 GetDeaths() const
	{
		return DeathsNum;
	}

	UFUNCTION(Server, Reliable)
		void AddDeath();
	
	UFUNCTION(Server, Reliable)
		void AddKill();
	
	UFUNCTION(Server, Reliable)
		void RemoveKill();
	
	FORCEINLINE int32 GetKills() const
	{
		return KillsNum;
	}
	
//private C++ variables
private:

	UPROPERTY(Replicated)
		int32 TeamID;
	FLinearColor TeamColor;

	UPROPERTY(ReplicatedUsing = OnRep_OnKillsNumChanged)
		int32 KillsNum = 0;

	UPROPERTY(ReplicatedUsing = OnRep_OnDeathsNumChanged)
		int32 DeathsNum = 0;

public:	
	
	UPROPERTY()
		AKT_BaseGameMode* GameMode;
};
