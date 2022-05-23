#pragma once

#include "KT_CoreTypes.generated.h"



USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
		int32 PlayersNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "3000"))
		int32 GameTime = 10;  // in seconds

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FLinearColor DefaultTeamColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FLinearColor> TeamColors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "20"))
		int32 RespawnTime;  // in seconds
};