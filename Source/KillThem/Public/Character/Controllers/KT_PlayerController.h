#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KT_PlayerController.generated.h"



UCLASS()
class KILLTHEM_API AKT_PlayerController : public APlayerController
{
	GENERATED_BODY()

	AKT_PlayerController();


//private c++ functions
private:


//protected c++ functions
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		virtual void OnPossess(APawn* InPawn) override;


//public c++ functions
public:

	UFUNCTION(Server, Reliable)
		void RespawnPlayer();

	UFUNCTION()
		void PlayerInitialize();

	
//Blueprint Values
public:

	UPROPERTY(BlueprintReadOnly)
		class AKT_PlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class AKT_GameHUD* GameHUD;
};
