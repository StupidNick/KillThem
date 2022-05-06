#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/KT_BaseProjectile.h"
#include "KT_RocketMissileProjectile.generated.h"



UCLASS()
class KILLTHEM_API AKT_RocketMissileProjectile : public AKT_BaseProjectile
{
	GENERATED_BODY()

//private c++ variables
private:

	UPROPERTY()
		TArray<AKT_BaseProjectile*> IgnoreProjectilesArray;

//protected c++ functions
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
		void SpawnMicroRockets();

//public BP variables
public:

	UPROPERTY(EditDefaultsOnly)
		int32 CountOfMissile;
	
	UPROPERTY(EditDefaultsOnly)
		float DelayBeforeSpawn;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AKT_BaseProjectile> ProjectileClass;
};
