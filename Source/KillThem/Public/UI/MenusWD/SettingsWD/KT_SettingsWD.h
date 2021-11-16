#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "KT_SettingsWD.generated.h"


class AKT_GameHUD;
class UButton;
UCLASS()
class KILLTHEM_API UKT_SettingsWD : public UUserWidget
{
	GENERATED_BODY()

//Constructor
public:

	UKT_SettingsWD(const FObjectInitializer& ObjectInitializer);
	



//c++ private values
public:

	

//c++ protected values
protected:


//c++ protected methods
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnEscapeButtonClick();


//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SettingsWD")
		void InitializeSettingsWD(AKT_GameHUD* InGameHUD);
		virtual void InitializeSettingsWD_Implementation(AKT_GameHUD* InGameHUD);


//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "SettingsWD|Widgets")
		UButton* EscapeButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "SettingsWD|Widgets")
		UTextBlock* GoAwayTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AKT_GameHUD* GameHUD = nullptr;
};
