// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KT_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KILLTHEM_API AKT_PlayerController : public APlayerController
{
	GENERATED_BODY()

	AKT_PlayerController();


//protected c++ functions
protected:
	
	virtual void BeginPlay() override;
};
