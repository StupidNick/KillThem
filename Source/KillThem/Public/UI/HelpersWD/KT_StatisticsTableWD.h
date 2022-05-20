#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/KT_PlayerState.h"
#include "KT_StatisticsTableWD.generated.h"


class UKT_StatisticsLineWD;
class UVerticalBox;
class UHorizontalBox;



UCLASS()
class KILLTHEM_API UKT_StatisticsTableWD : public UUserWidget
{
	GENERATED_BODY()

//public C++ functions	
public:
	
	UFUNCTION(BlueprintCallable)
		void InitializeWD(TArray<AKT_PlayerState*> TeammatesPlayerStates, TArray<AKT_PlayerState*> EnemiesPlayerStates, class AKT_TDMPlayerController* Controller);

	UFUNCTION(BlueprintCallable)
		void Update(const TArray<AKT_PlayerState*>& TeammatesPlayerStates, const TArray<AKT_PlayerState*>& EnemiesPlayerStates);

	UFUNCTION(BlueprintCallable)
		void ClearLists();
	
	UFUNCTION(BlueprintCallable)
		TArray<UKT_StatisticsLineWD*> SortListByKills(TArray<UKT_StatisticsLineWD*> UnsortedList);

//public BP variables
public:

	UPROPERTY(BlueprintReadOnly)
		class AKT_TDMPlayerController* MyController = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UHorizontalBox* RootHorizontalBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UVerticalBox* LeftVerticalBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UVerticalBox* RightVerticalBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UKT_StatisticsLineWD> StatisticLineClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		TArray<UKT_StatisticsLineWD*> ArrayOfStatisticLinesForTeammates;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		TArray<UKT_StatisticsLineWD*> ArrayOfStatisticLinesForEnemies;
};
