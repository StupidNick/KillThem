// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Controllers/KT_PlayerController.h"

#include "Character/KT_PlayerCharacter.h"


AKT_PlayerController::AKT_PlayerController()
{
	
}


void AKT_PlayerController::BeginPlay()
{
	if (Cast<AKT_PlayerCharacter>(GetPawn()))
	{
		Cast<AKT_PlayerCharacter>(GetPawn())->PlayerController = this;
	}
}