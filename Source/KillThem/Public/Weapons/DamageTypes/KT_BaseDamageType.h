#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "KT_BaseDamageType.generated.h"



UCLASS()
class KILLTHEM_API UKT_BaseDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageFactor;
};
