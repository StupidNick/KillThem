#include "UI/HelpersWD/Panels/KT_StatisticsLineWD.h"




void UKT_StatisticsLineWD::InitializeWD(const FText& InName, const FText& InKills, const FText& InDeaths,
	const FText& InPing, const int32& InKillsNum, const int32& InDeathNum)
{
	Name->SetText(InName);
	Kills->SetText(InKills);
	Deaths->SetText(InDeaths);
	Ping->SetText(InPing);
	KillsNum = InKillsNum;
	DeathsNum = InDeathNum;
}
