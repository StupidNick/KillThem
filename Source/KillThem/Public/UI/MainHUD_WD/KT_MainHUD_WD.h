#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Panels/KT_ProgressBarWithImageAndText.h"
#include "Panels/KT_ProgressBarWithText.h"
#include "KT_MainHUD_WD.generated.h"


class UKT_AimWD;
class UProgressBar;
class AKT_PlayerCharacter;
class UKT_IconWithTextWD;



UCLASS()
class KILLTHEM_API UKT_MainHUD_WD : public UUserWidget
{
	GENERATED_BODY()

//Constructor
public:

	UKT_MainHUD_WD(const FObjectInitializer& ObjectInitializer);
	



//c++ private values
public:

	

//c++ protected values
protected:


//c++ protected methods
protected:



//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MainHUD_WD")
		void InitializeMainHUD(AKT_PlayerCharacter* PlayerCharacter);
		virtual void InitializeMainHUD_Implementation(AKT_PlayerCharacter* PlayerCharacter);
	


	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | HPBar")
		void UpdateHP(float HPStat);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | SPBar")
		void UpdateSP(float SPStat);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | Weapon")
		void UpdateAmmo(int InAmmo);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | Weapon")
		void UpdateAmmoInTheClip(int InAmmo);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | Weapon")
		void UpdateIcon(UTexture2D* InWeaponIcon, UTexture2D* InAimIcon);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "MainHUD_WD | SPBar")
		void UpdateBooster(float Timer);


//Blueprint values
public:

	UPROPERTY(BlueprintReadOnly)
		AKT_PlayerCharacter* Character = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainHUD_WD | Widgets")
		UKT_ProgressBarWithText* HPBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainHUD_WD | Widgets")
		UKT_ProgressBarWithText* SPBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainHUD_WD | Widgets")
		UKT_IconWithTextWD* WeaponBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainHUD_WD | Widgets")
		UKT_AimWD* Aim = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainHUD_WD | Widgets")
		UKT_ProgressBarWithImageAndText* Ability = nullptr;
};
