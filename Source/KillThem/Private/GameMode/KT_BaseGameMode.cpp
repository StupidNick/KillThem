#include "GameMode/KT_BaseGameMode.h"

#include "Character/KT_PlayerCharacter.h"
#include "Components/KT_HealthComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"



AActor* AKT_BaseGameMode::ChoosePlayerStart_Implementation_Implementation(AController* Player)
{
		for (int LPlayerIndex = 0; LPlayerIndex < MaxPlayerCount - 1; LPlayerIndex ++)
		{
			for (const auto LPayerStart : PlayerStartArray)
			{
				if(LPayerStart->PlayerStartTag == FName(FString::FromInt(LPlayerIndex)))
				{
					const FTransform LSpawnTransform = LPayerStart->GetActorTransform();
					const FActorSpawnParameters LSpawnInfo;
				
					AKT_PlayerCharacter* LPawn = GetWorld()->SpawnActor<AKT_PlayerCharacter>(DefaultCharacterClass, LSpawnTransform.GetLocation(), LSpawnTransform.GetRotation().Rotator(), LSpawnInfo);
				
					if (IsValid(LPawn) && IsValid(Player))
					{
						Player->Possess(LPawn);
						Players.AddUnique(LPawn);
					}
					LPawn->HealthComponent->OnDead.AddDynamic(this, &AKT_BaseGameMode::RespawnPlayer);
				}
			}
		}
	return Player;
}


void AKT_BaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetPlayerStartPoints();
}


// const TSubclassOf<APlayerStart> LPlayerStartClass;
// TArray<AActor*> LPlayerStarts;
//
// UGameplayStatics::GetAllActorsOfClass(GetWorld(), LPlayerStartClass, LPlayerStarts);
//
// const int LRandStart = FMath::RandRange(0, LPlayerStarts.Num());
//
// const FVector LLocation = LPlayerStarts[LRandStart]->GetActorLocation();
// const FRotator LRotation = LPlayerStarts[LRandStart]->GetActorRotation();
// const FActorSpawnParameters LSpawnInfo;
//
// GetWorld()->SpawnActor<AKT_PlayerCharacter>(GetDefaultPawnClassForController(Player), LLocation, LRotation, LSpawnInfo);

void AKT_BaseGameMode::RespawnPlayer(AController* Player)
{
	FTimerHandle LTimerHandle;
	FTimerDelegate LTimerDelegate;

	LTimerDelegate.BindUFunction(this, "Respawn", Player);
	GetWorldTimerManager().SetTimer(LTimerHandle, LTimerDelegate, TimerForRespawnPlayers, false);
}


void AKT_BaseGameMode::Respawn(AController* Player)
{
	const int LRandPoint = FMath::RandRange(0, MaxPlayerCount);
	for (const auto LPayerStart : PlayerStartArray)
	{
		if(LPayerStart->PlayerStartTag == FName(FString::FromInt(LRandPoint)))
		{
			const FTransform LSpawnTransform = LPayerStart->GetActorTransform();
			const FActorSpawnParameters LSpawnInfo;
				
			AKT_PlayerCharacter* LPawn = GetWorld()->SpawnActor<AKT_PlayerCharacter>(DefaultCharacterClass, LSpawnTransform.GetLocation(), LSpawnTransform.GetRotation().Rotator(), LSpawnInfo);
				
			if (IsValid(LPawn) && IsValid(Player))
			{
				Player->Possess(LPawn);
			}
			LPawn->HealthComponent->OnDead.AddDynamic(this, &AKT_BaseGameMode::RespawnPlayer);
		}
	}
}


void AKT_BaseGameMode::GetPlayerStartPoints()
{
	TArray<AActor*> LPlayerStartArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartClassForFind, LPlayerStartArray);
	for (int i = 0; i < LPlayerStartArray.Num(); i++)
	{
		PlayerStartArray.Add(Cast<APlayerStart>(LPlayerStartArray[i]));
	}
}
