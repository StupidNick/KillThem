#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainHUD_WD/KT_ScreenOfDeathWD.h"
#include "UI/MenusWD/PauseMenuWD/KT_PauseMenuWD.h"
#include "UI/WeaponsWD/KT_SniperScopeWD.h"
#include "KT_GameHUD.generated.h"


class UKT_StartGameSpectatorWD;
class AKT_PlayerCharacter;
class UKT_SettingsWD;
class UKT_FindServerWD;
class UKT_MainMenuWD;
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

	UFUNCTION()
		void RespawnPlayer(AKT_PlayerCharacter* InPlayerCharacter);

//c++ prtotected methods
protected:


//c++ pivate methods
private:





//c++ public values
public:


//c++ protected values
protected:

	UFUNCTION()
		void AddFunctionToActiveWDArray(FString Name);

	UFUNCTION()
		void RemoveFunctionFromActiveWDArray(FString Name);

//c++ privat values
private:

	UPROPERTY()
		TArray<FString> ActiveWDNameArr;

//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void CreateMainHUD_WD();
	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void RemoveMainHUD_WD();

	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void CreateSniperScopeWD();
	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void RemoveSniperScopeWD();

	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void CreateScreenOfDeathWD(AKT_PlayerController* Controller, const FText& InKillerName, const int32& InRespawnTimer);
	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void RemoveScreenOfDeathWD();

	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void CreateSpectatorWD();
	UFUNCTION(BlueprintCallable, Category = "MainHUD")
		void RemoveSpectatorWD();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void CreateMainMenuWD();
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void RemoveMainMenuWD();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
		void CreatePauseMenuWD();
	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
		void RemovePauseMenuWD();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void CreateFindServerWD();
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void RemoveFindServerWD();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void CreateSettingsWD();
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void RemoveSettingsWD();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void CreateMadeByWD();
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void RemoveMadeByWD();


	UFUNCTION(BlueprintCallable, Category = "General")
		void RemoveAllActiveWD();


//Blueprint values
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_MainHUD_WD>  MainHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_MainHUD_WD* MainHUD = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_SniperScopeWD>  SniperScopeClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_SniperScopeWD* SniperScope = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_ScreenOfDeathWD>  ScreenOfDeathClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_ScreenOfDeathWD* ScreenOfDeath = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_StartGameSpectatorWD>  SpectatorWDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_StartGameSpectatorWD* SpectatorWD = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_MainMenuWD>  MainMenuClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_MainMenuWD* MainMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_PauseMenuWD>  PauseMenuClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_PauseMenuWD* PauseMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_FindServerWD>  FindServerClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_FindServerWD* FindServer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UKT_SettingsWD>  SettingsWDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UKT_SettingsWD* SettingsWD = nullptr;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	// 	TSubclassOf<UKT_MainMenuWD>  MainMenuClass;
	// UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	// 	UKT_MainMenuWD* MainMenu = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
		class AKT_PlayerController* MyController = nullptr;
	UPROPERTY(BlueprintReadOnly)
		AKT_PlayerCharacter* MyCharacter = nullptr;
	// UPROPERTY(BlueprintReadOnly)
	// 	class UKT_GameInstance* GameInstance = nullptr;
};
