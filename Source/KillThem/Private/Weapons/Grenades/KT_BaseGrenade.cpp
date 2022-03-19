#include "Weapons/Grenades/KT_BaseGrenade.h"

#include "Components/KT_ItemsManagerComponent.h"
#include "Weapons/Projectiles/KT_BaseProjectile.h"


AKT_BaseGrenade::AKT_BaseGrenade()
{
}


// void AKT_BaseGrenade::UseWeapon()
// {
// 	Super::UseWeapon();
//
// 	const FVector LLocation = Character->GetMesh()->GetSocketTransform(SpawnSocketName).GetLocation();
// 	const FRotator LRotation = Character->Controller->GetControlRotation();
// 	FActorSpawnParameters LSpawnInfo;
//
// 	LSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 	LSpawnInfo.Owner = this;
// 	LSpawnInfo.Instigator = Character;
//
// 	AKT_BaseProjectile* LProjectile = GetWorld()->SpawnActor<AKT_BaseProjectile>(ProjectileClass, LLocation, LRotation, LSpawnInfo);
// 	if (IsValid(LProjectile))
// 	{
// 		LProjectile->Initialize(Damage * Character->DamageBooster, Character);
// 	
// 		UE_LOG(LogTemp, Error, TEXT("SpawnGrenade"));
// 	}
// }