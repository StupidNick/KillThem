#include "GameMode/KT_BaseGameState.h"

#include "GameMode/KT_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"



void AKT_BaseGameState::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority()) return;
	
	MyGameMode = Cast<AKT_BaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameTimer = MyGameMode->GameTime;
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AKT_BaseGameState::GameTimerUpdate, 1.0f, true);
}


void AKT_BaseGameState::GameTimerUpdate()
{
	UE_LOG(LogTemp, Error, TEXT("Time: %i"), GameTimer);
	
	if (--GameTimer <= 0)
	{
		GetWorldTimerManager().ClearTimer(GameTimerHandle);
		TimeIsOver();
	}
}

