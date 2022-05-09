#include "GameMode/KT_BaseGameMode.h"

#include "EngineUtils.h"
#include "Character/KT_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/TransformCalculus3D.h"



AKT_BaseGameMode::AKT_BaseGameMode()
{
}


void AKT_BaseGameMode::StartPlay()
{
	Super::StartPlay();

	GameTime = GameData.GameTime;
	StartGame();
}


void AKT_BaseGameMode::StartGame()
{
	GameTimerUpdate();
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AKT_BaseGameMode::GameTimerUpdate, 1.0f, true);
}


void AKT_BaseGameMode::GameTimerUpdate()
{
	UE_LOG(LogTemp, Error, TEXT("Time: %i"), GameTime);
	
	if (--GameTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(GameTimerHandle);
	}
}


void AKT_BaseGameMode::BeginPlay()
{
	Super::BeginPlay();
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