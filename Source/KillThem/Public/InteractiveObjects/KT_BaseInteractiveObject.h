#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KT_BaseInteractiveObject.generated.h"

UCLASS()
class KILLTHEM_API AKT_BaseInteractiveObject : public AActor
{
	GENERATED_BODY()

//Constructors
public:	

	AKT_BaseInteractiveObject();

//private C++ functions
private:

	

	
	
//protected C++ functions
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Interactive(AActor* OtherActor);

	UFUNCTION(Server, Reliable)
		virtual void InteractiveOnServer(AActor* OtherActor);

	UFUNCTION()
		void EnableObject() const;

	FTimerHandle EnableTimerHandle;
	FTimerDelegate EnableTimerDelegate;
//public C++ functions
public:

	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		float RecoverTime;
};
