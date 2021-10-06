#pragma once

#include "CoreMinimal.h"


#include "Character/KT_PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "KT_BaseInteractiveObject.generated.h"



class UBoxComponent;
class USphereComponent;



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
		void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnActorDestroyed(AActor* InActor);

	virtual void Interactive(AActor* OtherActor);

	UFUNCTION(Server, Reliable)
		void InteractiveOnServer(AActor* OtherActor);

	UFUNCTION()
		void EnableObject();

	UFUNCTION()
		void DisableObject();


	FTimerHandle EnableTimerHandle;
	FTimerDelegate EnableTimerDelegate;

//protected C++ variables
protected:

	bool CanTake = true;
	
//public C++ functions
public:

	UFUNCTION()
		void ToInteractive(AActor* OtherActor);
	
//public BP variables
public:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TArray<AKT_PlayerCharacter*> Players;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USphereComponent* InteractSphereCollision = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "WorkInfo")
		float RecoverTime;

	UPROPERTY(EditDefaultsOnly, Category = "WorkInfo")
		bool CanInteractOnOverlap;
};
