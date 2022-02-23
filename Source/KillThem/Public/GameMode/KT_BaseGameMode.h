#pragma once

#include "CoreMinimal.h"
#include "Character/KT_PlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "KT_BaseGameMode.generated.h"


UCLASS()
class KILLTHEM_API AKT_BaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

//public C++ methods
public:

	UFUNCTION(Reliable, Server)
		void RespawnPlayer(AController* Player);

	UFUNCTION()
		void Respawn(AController* Player);

//public C++ variables
public:

	UPROPERTY()
		TArray<APlayerStart*> PlayerStartArray;

	UPROPERTY()
		TArray<AKT_PlayerCharacter*> Players;

//protected C++ functions
protected:
	
	virtual void BeginPlay() override;
	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		int MaxPlayerCount;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		float TimerForRespawnPlayers;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		TSubclassOf<AActor> PlayerStartClassForFind;

	UPROPERTY(EditDefaultsOnly, Category = "Players")
		TSubclassOf<AKT_PlayerCharacter> DefaultCharacterClass;
};