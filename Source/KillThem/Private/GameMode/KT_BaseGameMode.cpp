#include "GameMode/KT_BaseGameMode.h"

#include "EngineUtils.h"
#include "Character/KT_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/TransformCalculus3D.h"



AKT_BaseGameMode::AKT_BaseGameMode()
{
}


void AKT_BaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	// for (AKT_PlayerController* PlayerController : TActorRange<AKT_PlayerController>(GetWorld()))
	// {
	// 	RespawnPlayer(PlayerController);
	// }
}


void AKT_BaseGameMode::RespawnPlayer_Implementation(AController* Player)
{
	if (!IsValid(Player) || !HasAuthority()) return;
	
	const FTransform LSpawnTransform = ChoosePlayerStart(Player)->GetActorTransform();
	const FActorSpawnParameters LSpawnInfo;
	AKT_PlayerCharacter* LPawn = GetWorld()->SpawnActor<AKT_PlayerCharacter>(DefaultCharacterClass, LSpawnTransform.GetLocation(), LSpawnTransform.GetRotation().Rotator(), LSpawnInfo);

	if (IsValid(LPawn))
	{
		Player->Possess(LPawn);
	}
}


void AKT_BaseGameMode::Respawn(AController* Player)
{
	// const int LRandPoint = FMath::RandRange(0, MaxPlayerCount);
	// for (const auto LPayerStart : PlayerStartArray)
	// {
	// 	if(LPayerStart->PlayerStartTag == FName(FString::FromInt(LRandPoint)))
	// 	{
	// 		const FTransform LSpawnTransform = LPayerStart->GetActorTransform();
	// 		const FActorSpawnParameters LSpawnInfo;
	// 			
	// 		AKT_PlayerCharacter* LPawn = GetWorld()->SpawnActor<AKT_PlayerCharacter>(DefaultCharacterClass, LSpawnTransform.GetLocation(), LSpawnTransform.GetRotation().Rotator(), LSpawnInfo);
	// 			
	// 		if (IsValid(LPawn) && IsValid(Player))
	// 		{
	// 			Player->Possess(LPawn);
	// 		}
	// 		LPawn->HealthComponent->OnDead.AddDynamic(this, &AKT_BaseGameMode::RespawnPlayer);
	// 	}
	// }
}
