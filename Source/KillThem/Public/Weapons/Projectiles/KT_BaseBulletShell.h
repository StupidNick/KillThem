#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KT_BaseBulletShell.generated.h"



UCLASS()
class KILLTHEM_API AKT_BaseBulletShell : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AKT_BaseBulletShell();

protected:
	
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void Destruction();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Components")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Timers")
		float DestructionTimer = 5;
};
