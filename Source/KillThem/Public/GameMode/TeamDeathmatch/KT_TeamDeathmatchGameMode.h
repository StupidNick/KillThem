#pragma once

#include "CoreMinimal.h"
#include "GameMode/KT_BaseGameMode.h"
#include "KT_TeamDeathmatchGameMode.generated.h"




UCLASS()
class KILLTHEM_API AKT_TeamDeathmatchGameMode : public AKT_BaseGameMode
{
	GENERATED_BODY()

//public C++ functions
public:

	virtual void Killed(const AController* KilledController, const AController* VictimController) override;
	
	UFUNCTION(Server, Reliable)
		void SetTeamsInfo(AKT_PlayerController* Controller);

	UFUNCTION()
		TArray<AKT_PlayerState*> GetTeammates(const AKT_PlayerController* Controller) const;

	UFUNCTION()
		TArray<AKT_PlayerState*> GetEnemies(const AKT_PlayerController* Controller) const;

	virtual bool IsTeammates(const AController* Controller1, const AController* Controller2) const override;
	
	virtual void UpdateStatistic() override;

	
//private C++ functions
private:

	UFUNCTION()
		FLinearColor DetermineColorByTeamID(int32 InTeamID) const;
	
	UFUNCTION(Server, Reliable)
		void SetPlayerColor(AController* Controller);

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	
//private C++ variables
private:

	int32 TeamID = 0;


//public BP variables
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "3"))
		int32 NumbersOfTeam = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FLinearColor> TeamColors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FLinearColor DefaultTeamColor = FLinearColor::Red;
};
