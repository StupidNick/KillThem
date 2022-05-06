#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/KT_BaseProjectile.h"
#include "KT_HomingMissileProjectile.generated.h"




UCLASS()
class KILLTHEM_API AKT_HomingMissileProjectile : public AKT_BaseProjectile
{
	GENERATED_BODY()

public:
	
	AKT_HomingMissileProjectile();

	virtual void Tick(float DeltaSeconds) override;

//protected c++ functions
protected:
	
	bool GetTraceData(FVector& StartLocation, FVector& EndLocation) const;

	void SetRotationToTarget() const;
	
	void SetNewVelocity();

	void MakeHit(FHitResult& HitResult, const FVector& StartLocation, const FVector& EndLocation) const;
	
//protected c++ functions
protected:

	float CurrentSpeed = 100.0f;

//public BP variables
public:
	
	UPROPERTY(EditDefaultsOnly)
		float MaxDistanceAttack;

	UPROPERTY(EditDefaultsOnly)
		float RotationSpeed;

	UPROPERTY(EditDefaultsOnly)
		float AccelerationSpeed;

	UPROPERTY(EditDefaultsOnly)
		float StarterDelay;
};
