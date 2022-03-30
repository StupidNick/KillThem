#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KT_PlayerStart.generated.h"

UCLASS()
class KILLTHEM_API AKT_PlayerStart : public AActor
{
	GENERATED_BODY()

public:	
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Players")
		TArray<FTransform> PlayerStartArray;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Players")
		int32 PlayerStartCounter;
};
