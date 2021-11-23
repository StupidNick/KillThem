#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TimelineComponent.h"
#include "KT_ProgressBarWithImageAndText.generated.h"



class UTextBlock;
class UProgressBar;



UCLASS()
class KILLTHEM_API UKT_ProgressBarWithImageAndText : public UUserWidget
{
	GENERATED_BODY()

//Constructor
public:

	UKT_ProgressBarWithImageAndText(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

//c++ private values
private:
	

//c++ private values
private:


//c++ protected values
protected:


//c++ protected methods
protected:



//public C++ variables
public:

	UPROPERTY()
		float TimerTime;
	

//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProgressBarWithImageAndText | Widgets")
		UProgressBar* ValueProgressBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProgressBarWithImageAndText | Widgets")
		UTextBlock* ValueTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProgressBarWithImageAndText | Widgets")
		UImage* BackgroundImage = nullptr;
};
