#pragma once

#include "CoreMinimal.h"
#include "KT_BaseGameState.h"
#include "KT_PlayerState.h"
#include "Character/KT_PlayerCharacter.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "KT_BaseGameMode.generated.h"


UCLASS()
class KILLTHEM_API AKT_BaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

//public C++ methods
public:
	
	AKT_BaseGameMode();

	UFUNCTION(Reliable, Server)
		void RespawnPlayer(AController* Player);

	virtual void StartPlay() override;

	UFUNCTION()
		TArray<AKT_PlayerState*> FindPlayerStates() const;

	UFUNCTION()
		virtual void Killed(const AController* KilledController, const AController* VictimController);

	UFUNCTION()
		void GameOver(const FName& WinnerName);

	UFUNCTION()
		virtual bool IsTeammates(const AController* Controller1, const AController* Controller2) const;

	UFUNCTION()
		virtual void UpdateStatistic(){};

//private C++ functions
private:

	UFUNCTION()
		void StartGame();


//protected C++ functions
protected:

    
	
	virtual void BeginPlay() override;
	
//public BP variables
public:

	// UPROPERTY(EditDefaultsOnly, Category = "Game")
	// 	FGameData GameData;

	UPROPERTY()
		AKT_BaseGameState* MyGameState;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		TSubclassOf<AKT_PlayerCharacter> DefaultCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		TSubclassOf<AKT_PlayerController> DefaultControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "0", ClampMax = "100"))
		int32 DamageToTeammatesAsPercent = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "3000"))
		int32 GameTime = 10;  // in seconds

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "50"))
		int32 NumbersOfPlayer = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "-1", ClampMax = "300"))
		int32 WinScore = 2;
};