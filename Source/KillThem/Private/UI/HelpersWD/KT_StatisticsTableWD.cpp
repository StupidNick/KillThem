#include "UI/HelpersWD/KT_StatisticsTableWD.h"

#include "Character/Controllers/KT_TDMPlayerController.h"
#include "Components/VerticalBox.h"
#include "UI/HelpersWD/Panels/KT_StatisticsLineWD.h"


void UKT_StatisticsTableWD::InitializeWD(TArray<AKT_PlayerState*> TeammatesPlayerStates,
                                         TArray<AKT_PlayerState*> EnemiesPlayerStates, AKT_TDMPlayerController* Controller)
{
	if (!StatisticLineClass || !TeammatesPlayerStates.IsValidIndex(0) || !EnemiesPlayerStates.IsValidIndex(0) || !IsValid(Controller)) return;//TODO correct code

	MyController = Controller;
	MyController->TDMStatisticTableUpdate.AddDynamic(this, &UKT_StatisticsTableWD::Update);
	
	Update(TeammatesPlayerStates, EnemiesPlayerStates);
}


void UKT_StatisticsTableWD::Update(const TArray<AKT_PlayerState*>& TeammatesPlayerStates,
									const TArray<AKT_PlayerState*>& EnemiesPlayerStates) //TODO refactor this shit
{
	ClearLists();
	TArray<UKT_StatisticsLineWD*> LUnsortedList;
	for (int32 LineIndex = 0; LineIndex < TeammatesPlayerStates.Num(); ++LineIndex)
	{
		const int32 LKills = TeammatesPlayerStates[LineIndex]->GetKills();
		const int32 LDeaths = TeammatesPlayerStates[LineIndex]->GetDeaths();
		
		UKT_StatisticsLineWD* LLine = CreateWidget<UKT_StatisticsLineWD>(GetWorld(), StatisticLineClass);
		const FText LName = FText::FromString(TeammatesPlayerStates[LineIndex]->GetName());
		const FText LKillsText = FText::AsNumber(LKills);
		const FText LDeathsText = FText::AsNumber(LDeaths);
		LLine->InitializeWD(LName, LKillsText, LDeathsText, FText::FromString("45"), LKills, LDeaths);//TODO change ping to real users ping
		LUnsortedList.Add(LLine);
	}
	for (const auto Line : SortListByKills(LUnsortedList))
	{
		LeftVerticalBox->AddChild(Line);
	}
	LUnsortedList.Empty();
	for (int32 LineIndex = 0; LineIndex < EnemiesPlayerStates.Num(); ++LineIndex)
	{
		const int32 LKills = TeammatesPlayerStates[LineIndex]->GetKills();
		const int32 LDeaths = TeammatesPlayerStates[LineIndex]->GetDeaths();
		
		UKT_StatisticsLineWD* LLine = CreateWidget<UKT_StatisticsLineWD>(GetWorld(), StatisticLineClass);
		const FText LName = FText::FromString(EnemiesPlayerStates[LineIndex]->GetName());
		const FText LKillsText = FText::AsNumber(LKills);
		const FText LDeathsText = FText::AsNumber(LDeaths);
		LLine->InitializeWD(LName, LKillsText, LDeathsText, FText::FromString("45"), LKills, LDeaths);//TODO change ping to real users ping
		LUnsortedList.Add(LLine);
	}
	for (const auto Line : SortListByKills(LUnsortedList))
	{
		RightVerticalBox->AddChild(Line);
	}
}


void UKT_StatisticsTableWD::ClearLists()
{
	for (const auto Line : LeftVerticalBox->GetAllChildren())
	{
		Line->RemoveFromParent();
	}
	for (const auto Line : RightVerticalBox->GetAllChildren())
	{
		Line->RemoveFromParent();
	}
}


TArray<UKT_StatisticsLineWD*> UKT_StatisticsTableWD::SortListByKills(TArray<UKT_StatisticsLineWD*> UnsortedList)
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
