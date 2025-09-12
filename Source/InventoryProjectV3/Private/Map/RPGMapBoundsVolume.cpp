// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapBoundsVolume.h"

// Sets default values
ARPGMapBoundsVolume::ARPGMapBoundsVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPGMapBoundsVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGMapBoundsVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

