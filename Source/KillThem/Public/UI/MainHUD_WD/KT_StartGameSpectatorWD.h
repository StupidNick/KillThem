#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "Components/TextBlock.h"
#include "KT_StartGameSpectatorWD.generated.h"



UCLASS()
class KILLTHEM_API UKT_StartGameSpectatorWD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void InitializeWD(AKT_PlayerController* Controller);

//private C++ functions	
private:	
	
	UFUNCTION()
		void SpawnPlayer();
	
//private C++ functions	
private:

	
	
//public BP variables	
public:
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* ReadyToGameTextBlock;

	UPROPERTY()
		AKT_PlayerController* MyController;
};
