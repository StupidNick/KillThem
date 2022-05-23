#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KT_DeathmatchStatisticTable.generated.h"



class AKT_PlayerState;
class UHorizontalBox;
class UVerticalBox;
class UKT_StatisticsLineWD;



UCLASS()
class KILLTHEM_API UKT_DeathmatchStatisticTable : public UUserWidget
{
	GENERATED_BODY()


//public C++ functions	
public:
	
	UFUNCTION(BlueprintCallable)
		void InitializeWD(TArray<AKT_PlayerState*> PlayerStatesArray, class AKT_DeathmatchPlayerController* Controller);

	UFUNCTION(BlueprintCallable)
		void Update(const TArray<AKT_PlayerState*>& PlayerStatesArray);

	UFUNCTION(BlueprintCallable)
		void ClearLists();
	
	UFUNCTION(BlueprintCallable)
		TArray<UKT_StatisticsLineWD*> SortListByKills(TArray<UKT_StatisticsLineWD*> UnsortedList);

//public BP variables
public:

	UPROPERTY(BlueprintReadOnly)
		class AKT_DeathmatchPlayerController* MyController = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UHorizontalBox* RootHorizontalBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UVerticalBox* MainVerticalBox = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UKT_StatisticsLineWD> StatisticLineClass;	
};
