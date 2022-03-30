#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KT_DashProgressBar.generated.h"



class UProgressBar;



UCLASS()
class KILLTHEM_API UKT_DashProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProgressBar")
		UProgressBar* ValueProgressBar = nullptr;
};
