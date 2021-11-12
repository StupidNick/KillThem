#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "KT_TypesOfAmmoEnum.generated.h"



UENUM(BlueprintType)
enum AmmoType
{
	SMG				UMETA(DisplayName = "SMG"),
	Rifle			UMETA(DisplayName = "Rifle"),
	SniperRifle		UMETA(DisplayName = "SniperRifle"),
  };