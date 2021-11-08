#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KT_IconWithTextWD.generated.h"



class UImage;
class UTextBlock;



UCLASS()
class KILLTHEM_API UKT_IconWithTextWD : public UUserWidget
{
	GENERATED_BODY()
	
//Constructor
public:

	UKT_IconWithTextWD(const FObjectInitializer& ObjectInitializer);


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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "IconWithTextWD | Widgets")
		UTextBlock* TextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "IconWithTextWD | Widgets")
		UImage* Icon = nullptr;
};