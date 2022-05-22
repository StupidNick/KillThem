#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Controllers/KT_PlayerController.h"
#include "Components/TextBlock.h"
#include "StatisticWD/KT_DeathmatchStatisticTable.h"
#include "KT_DMWinWD.generated.h"




UCLASS()
class KILLTHEM_API UKT_DMWinWD : public UUserWidget
{
	GENERATED_BODY()

//public C++ functions	
public:	

	void InitializeWD(TArray<AKT_PlayerState*> PlayerStatesArray, AKT_PlayerController* Controller, const FString& WinnerName) const;

	
//public BP variables	
public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* GameOverTextBlock;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* WinTextBlock;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Widgets")
		UKT_DeathmatchStatisticTable* DeathmatchStatisticTable;

	UPROPERTY(EditDefaultsOnly, Category = "GameOverTexts")
		FString WinText;
	UPROPERTY(EditDefaultsOnly, Category = "GameOverTexts")
		FString LoseText;
	UPROPERTY(EditDefaultsOnly, Category = "GameOverTexts")
		FString PlayerWinText;
};
