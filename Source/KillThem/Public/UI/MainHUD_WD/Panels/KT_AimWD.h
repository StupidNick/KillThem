#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KT_AimWD.generated.h"



class UImage;



UCLASS()
class KILLTHEM_API UKT_AimWD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UKT_AimWD(const FObjectInitializer& ObjectInitializer);


//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainHUD_WD | Widgets")
		UImage* Aim = nullptr;	
};
