#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "KT_ProgressBarWithText.generated.h"



class UProgressBar;



UCLASS()
class KILLTHEM_API UKT_ProgressBarWithText : public UUserWidget
{
	GENERATED_BODY()
	
//Constructor
public:

	UKT_ProgressBarWithText(const FObjectInitializer& ObjectInitializer);


//c++ private values
public:

	

//c++ protected values
protected:


//c++ protected methods
protected:



//Blueprint methods
public:


//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProgressBarWithText | Widgets")
		UProgressBar* ValueProgressBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProgressBarWithText | Widgets")
		UTextBlock* ValueTextBlock = nullptr;
};
