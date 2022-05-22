#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "KT_StatisticsLineWD.generated.h"



UCLASS()
class KILLTHEM_API UKT_StatisticsLineWD : public UUserWidget
{
	GENERATED_BODY()

//public C++ functions	
public:

	UPROPERTY()
		int32 KillsNum;
	
	UPROPERTY()
		int32 DeathsNum;

//public C++ functions	
public:

	void InitializeWD(const FText& InName, const FText& InKills, const FText& InDeaths, const FText& InPing, const int32& InKillsNum, const int32& InDeathNum);

	FORCEINLINE void SetName(const FText& InName) const
	{
		Name->SetText(InName);
	}
	
	FORCEINLINE void SetKills(const FText& InKills) const
	{
		Kills->SetText(InKills);
	}
	
	FORCEINLINE void SetDeaths(const FText& InDeaths) const
	{
		Deaths->SetText(InDeaths);
	}
	
	FORCEINLINE void SetPing(const FText& InPing) const
	{
		Ping->SetText(InPing);
	}
	
	
//public BP variables
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* Name = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* Kills = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* Deaths = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
		UTextBlock* Ping = nullptr;
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Widgets")
	// 	UTextBlock* Asists = nullptr;
};
