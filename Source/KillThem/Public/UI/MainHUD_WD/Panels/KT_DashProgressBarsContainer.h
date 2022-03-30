#pragma once

#include "CoreMinimal.h"
#include "KT_DashProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"
#include "KT_DashProgressBarsContainer.generated.h"



class AKT_PlayerCharacter;



UCLASS()
class KILLTHEM_API UKT_DashProgressBarsContainer : public UUserWidget
{
	GENERATED_BODY()

	UKT_DashProgressBarsContainer(const FObjectInitializer& ObjectInitializer);
	
//Private C++ functions
private:

	UPROPERTY()
		TArray<UKT_DashProgressBar*> ProgressBars;

//private C++ variables
private:

	FTimerHandle HideTimerHandle;
	FTimerDelegate HideTimerDelegate;
	
//Public C++ functions
public:

	UFUNCTION()
		void InitializeProgressBarsContainer(const AKT_PlayerCharacter* Player);

	UFUNCTION()
		void UpdateProgressBar(const float& Value, const int32& Index);

	UFUNCTION(BlueprintCosmetic)
		void ClearProgressBar(const int32& Index);

	UFUNCTION(BlueprintNativeEvent)
		void HideContainer();
		void HideContainer_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void UnHideContainer();
		void UnHideContainer_Implementation();

//Public BP variables
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "MainHUD_WD | Widgets")
		TSubclassOf<UKT_DashProgressBar> ProgressBarClass;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "MainHUD_WD | Widgets")
		UHorizontalBox* HorizontalBox;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float ProgressBarPaddings;

	UPROPERTY(BlueprintReadOnly, Category = "Settings")
		bool bIsVisible = true;
};
