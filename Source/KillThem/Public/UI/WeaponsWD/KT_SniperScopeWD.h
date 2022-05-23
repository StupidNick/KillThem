#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "KT_SniperScopeWD.generated.h"



UCLASS()
class KILLTHEM_API UKT_SniperScopeWD : public UUserWidget
{
	GENERATED_BODY()

//public BP variables
public:	
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UImage* ScopeImage;
};
