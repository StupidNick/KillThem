#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "KT_ScreenOfDeathWD.generated.h"



class UTextBlock;



UCLASS()
class KILLTHEM_API UKT_ScreenOfDeathWD : public UUserWidget
{
	GENERATED_BODY()

//Blueprint methods
public:

	UFUNCTION(BlueprintCallable)
		void InitializeWD(AKT_PlayerController* Controller, const FText& InKillerName, const int32& InRespawnTimer);

	UFUNCTION(BlueprintCallable)
		void UpdateWD(const int32& InRespawnTimer);

	UFUNCTION(BlueprintCallable)
		void SetReadyRespawn(const bool CanRespawn);

	UFUNCTION(BlueprintCallable)
		void RespawnPlayer();

//Blueprint values
public:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* KillerName = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* RespawnTimer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
		AKT_PlayerController* MyController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FText ReadyToRespawnText;
};
