#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KT_MainMenuWD.generated.h"


class AKT_GameHUD;
class UButton;
UCLASS()
class KILLTHEM_API UKT_MainMenuWD : public UUserWidget
{
	GENERATED_BODY()

	//Constructor
public:

	UKT_MainMenuWD(const FObjectInitializer& ObjectInitializer);
	



//c++ private values
public:

	

//c++ protected values
protected:


//c++ protected methods
protected:

	virtual void NativeConstruct() override;

///////////////////On Buttons clicked////////////////////////////

	UFUNCTION()
		void OnFindServerButtonClick();
	UFUNCTION()
		void OnSettingsButtonClick();
	UFUNCTION()
		void OnMadeByButtonClick();
	UFUNCTION()
		void OnExitButtonClick();


//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MainMenuWD")
		void InitializeMainMenu(AKT_GameHUD* InGameHUD);
		virtual void InitializeMainMenu_Implementation(AKT_GameHUD* InGameHUD);


//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainMenuWD|Widgets")
		UButton* FindServerButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainMenuWD|Widgets")
		UButton* SettingsButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainMenuWD|Widgets")
		UButton* MadeByButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainMenuWD|Widgets")
		UButton* ExitButton = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainMenuWD")
		FText EscapeToDesktop_ConfirmationText;
	

	UPROPERTY(BlueprintReadOnly)
		AKT_GameHUD* GameHUD = nullptr;
};
