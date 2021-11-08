#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KT_PlayerController.generated.h"




UCLASS()
class KILLTHEM_API AKT_PlayerController : public APlayerController
{
	GENERATED_BODY()

	AKT_PlayerController();


//protected c++ functions
protected:
	
	virtual void BeginPlay() override;

	
//Blueprint Values
public:

	UPROPERTY(BlueprintReadOnly)
		class AKT_PlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class AKT_GameHUD* GameHUD;
};
