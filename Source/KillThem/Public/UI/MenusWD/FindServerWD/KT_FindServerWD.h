#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KT_FindServerWD.generated.h"


class AKT_GameHUD;
class UButton;



UCLASS()
class KILLTHEM_API UKT_FindServerWD : public UUserWidget
{
	GENERATED_BODY()

//Constructor
public:

	UKT_FindServerWD(const FObjectInitializer& ObjectInitializer);
	



//c++ private values
public:

	

//c++ protected values
protected:


//c++ protected methods
protected:

	virtual void NativeConstruct() override;


//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FindServerWD")
		void InitializeFindServerWD(AKT_GameHUD* InGameHUD);
		virtual void InitializeFindServerWD_Implementation(AKT_GameHUD* InGameHUD);
	


	UFUNCTION(BlueprintCallable, Category = "FindServerWD")
		void OnExitButtonClick();



//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "FindServerWD|Widgets")
		UButton* ExitButton = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AKT_GameHUD* GameHUD = nullptr;
};
