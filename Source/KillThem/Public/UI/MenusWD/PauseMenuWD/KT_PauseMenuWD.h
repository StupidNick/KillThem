#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KT_PauseMenuWD.generated.h"


class UButton;
class AKT_GameHUD;


UCLASS()
class KILLTHEM_API UKT_PauseMenuWD : public UUserWidget
{
	GENERATED_BODY()


//Constructor
public:

	UKT_PauseMenuWD(const FObjectInitializer& ObjectInitializer);


//c++ private values
private:

	

//c++ protected values
protected:

//c++ protected methods
protected:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

///////////////////On Buttons clicked////////////////////////////

	UFUNCTION()
		void OnResumeButtonClick();
	UFUNCTION()
		void OnSettingsButtonClick();
	UFUNCTION()
		void OnExitToMenuButtonClick();
	UFUNCTION()
		void OnExitGameButtonClick();


//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PauseMenu")
		void InitializeMainMenu(AKT_GameHUD* InGameHUD);
		virtual void InitializeMainMenu_Implementation(AKT_GameHUD* InGameHUD);


//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "PauseMenu | Widgets")
		UButton* ResumeButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "PauseMenu | Widgets")
		UButton* SettingsButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "PauseMenu | Widgets")
		UButton* ExitToMenuButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "PauseMenu | Widgets")
		UButton* ExitGameButton = nullptr;
	

	UPROPERTY(BlueprintReadOnly)
		AKT_GameHUD* GameHUD = nullptr;

	UPROPERTY(EditAnywhere, Category = "PauseMenu | Info")
		FName MainMenuLevelName;
};
