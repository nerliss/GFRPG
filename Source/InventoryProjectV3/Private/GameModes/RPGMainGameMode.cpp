// Oleksandr Tkachov 2021-2026


#include "GameModes/RPGMainGameMode.h"

#include "Map/RPGMapSubsystem.h"

ARPGMainGameMode::ARPGMainGameMode()
{
	MapTexture = nullptr;
	FogRT = nullptr;
}

void ARPGMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Map subsystem (probably not the best place for it, will do for now)
	if (URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>())
	{
		// TODO: Move all these Map-related settings somewhere else
		//MapSubsystem->FindAndSetBoundsByActorTag("Map.Primary");
		MapSubsystem->MapTexture = TSoftObjectPtr<UTexture2D>(MapTexture);
		MapSubsystem->FogRT = FogRT;
	}
}
