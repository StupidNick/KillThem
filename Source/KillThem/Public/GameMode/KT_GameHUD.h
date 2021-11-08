#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KT_GameHUD.generated.h"



class UKT_MainHUD_WD;



UCLASS()
class KILLTHEM_API AKT_GameHUD : public AHUD
{
	GENERATED_BODY()


//Constructor
public:

	AKT_GameHUD();
	


//c++ public methods
public:

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;


//c++ prtotected methods
protected:


//c++ pivate methods
private:





//c++ public values
public:


//c++ protected values
protected:
	

//c++ privat values
private:


//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void CreateMainHUD();
	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void RemoveMainHUD();


//Blueprint values
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_MainHUD_WD>  MainHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_MainHUD_WD* MainHUD = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
		class AKT_PlayerController* MyController = nullptr;
	// UPROPERTY(BlueprintReadOnly)
	// 	class UKT_GameInstance* GameInstance = nullptr;
};
