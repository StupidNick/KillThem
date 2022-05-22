#include "UI/GameModeWD/StatisticWD/KT_DeathmatchStatisticTable.h"

#include "Character/Controllers/KT_DeathmatchPlayerController.h"
#include "Components/VerticalBox.h"
#include "GameMode/KT_PlayerState.h"
#include "UI/GameModeWD/StatisticWD/Panels/KT_StatisticsLineWD.h"


void UKT_DeathmatchStatisticTable::InitializeWD(TArray<AKT_PlayerState*> PlayerStatesArray, AKT_DeathmatchPlayerController* Controller)
{
	if (!StatisticLineClass || !PlayerStatesArray.IsValidIndex(0) || !IsValid(Controller)) return;//TODO correct code

	MyController = Controller;
	MyController->DMStatisticTableUpdate.AddDynamic(this, &UKT_DeathmatchStatisticTable::Update);
	
	Update(PlayerStatesArray);
}


void UKT_DeathmatchStatisticTable::Update(const TArray<AKT_PlayerState*>& PlayerStatesArray)
{
	ClearLists();
	TArray<UKT_StatisticsLineWD*> LUnsortedList;
	for (int32 LineIndex = 0; LineIndex < PlayerStatesArray.Num(); ++LineIndex)
	{
		const int32 LKills = PlayerStatesArray[LineIndex]->GetKills();
		const int32 LDeaths = PlayerStatesArray[LineIndex]->GetDeaths();
		
		UKT_StatisticsLineWD* LLine = CreateWidget<UKT_StatisticsLineWD>(GetWorld(), StatisticLineClass);
		if (!IsValid(LLine)) continue;
		
		const FText LName = FText::FromString(PlayerStatesArray[LineIndex]->GetName());
		const FText LKillsText = FText::AsNumber(LKills);
		const FText LDeathsText = FText::AsNumber(LDeaths);
		LLine->InitializeWD(LName, LKillsText, LDeathsText, FText::FromString("45"), LKills, LDeaths);//TODO change ping to real users ping
		LUnsortedList.Add(LLine);
	}
	for (const auto Line : SortListByKills(LUnsortedList))
	{
		MainVerticalBox->AddChild(Line);
	}
}


void UKT_DeathmatchStatisticTable::ClearLists()
{
	for (const auto Line : MainVerticalBox->GetAllChildren())
	{
		Line->RemoveFromParent();
	}
}


TArray<UKT_StatisticsLineWD*> UKT_DeathmatchStatisticTable::SortListByKills(TArray<UKT_StatisticsLineWD*> UnsortedList)
{
	TMap<UKT_StatisticsLineWD*, int32> LUnsortedTMap;
	for (auto Line : UnsortedList)
	{
		LUnsortedTMap.Add(Line, Line->KillsNum);
	}
	LUnsortedTMap.ValueSort([](const int32& A, const int32& B)
	{
		return A > B;
	});

	TArray<UKT_StatisticsLineWD*> LSortedList;
	for (auto Map : LUnsortedTMap)
	{
		LSortedList.Add(Map.Key);
	}
	return LSortedList;
}