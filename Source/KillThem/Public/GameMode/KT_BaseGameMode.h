#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION(Server, Reliable)
		void CreateTeamsInfo();

//private C++ variables
private:

	FTimerHandle GameTimerHandle;
	int32 GameTimer = 0;

//private C++ functions
private:
	
	UFUNCTION(Server, Reliable)
		void CreateControllers();

	UFUNCTION()
		void StartGame();

	UFUNCTION()
		void GameTimerUpdate();

	UFUNCTION()
		void GameOver();
	
	UFUNCTION()
		FLinearColor DetermineColorByTeamID(int32 TeamID) const;
	
	UFUNCTION(Server, Reliable)
		void SetPlayerColor(AController* Controller);

//protected C++ functions
protected:
	
	virtual void BeginPlay() override;
	
//public BP variables
public:

	// UPROPERTY(EditDefaultsOnly, Category = "Game")
	// 	FGameData GameData;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		TSubclassOf<AKT_PlayerCharacter> DefaultCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		TSubclassOf<AKT_PlayerController> DefaultControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "3000"))
		int32 GameTime = 10;  // in seconds

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "3"))
		int32 NumbersOfTeam = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "50"))
		int32 NumbersOfPlayer = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FLinearColor> TeamColors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FLinearColor DefaultTeamColor = FLinearColor::Red;
};