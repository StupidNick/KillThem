#include "Weapons/RangeWeapon/KT_WeaponSniperRifle.h"

#include "Camera/CameraComponent.h"
#include "GameMode/KT_GameHUD.h"
#include "UI/MainHUD_WD/KT_MainHUD_WD.h"


void AKT_WeaponSniperRifle::Scope()
{
	if (!IsValid(Character)) return;
	
	ActualFoV = Character->CameraComponent->FieldOfView;
	Character->HUD->CreateSniperScopeWD();
	Character->HUD->MainHUD->RemoveFromParent();
	Character->CameraComponent->SetFieldOfView(ScopeFOV);
}


void AKT_WeaponSniperRifle::UnScope()
{
	if (!IsValid(Character)) return;
	
	Character->CameraComponent->SetFieldOfView(ActualFoV);
	Character->HUD->RemoveSniperScopeWD();
	Character->HUD->MainHUD->AddToViewport();
}