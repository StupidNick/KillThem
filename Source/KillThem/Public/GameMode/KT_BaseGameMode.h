#pragma once

#include "CoreMinimal.h"
#include "KT_CoreTypes.h"
#include "Character/KT_PlayerCharacter.h"
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

//private C++ variables
private:

	FTimerHandle GameTimerHandle;
	int32 GameTime = 0;

//private C++ functions
private:
	
	UFUNCTION()
		void StartGame();

	UFUNCTION()
		void GameTimerUpdate();


//protected C++ functions
protected:
	
	virtual void BeginPlay() override;
	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Game")
		FGameData GameData;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		TSubclassOf<AKT_PlayerCharacter> DefaultCharacterClass;
};