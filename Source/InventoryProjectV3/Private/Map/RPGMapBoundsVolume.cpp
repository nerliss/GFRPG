// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapBoundsVolume.h"

#include "Components/BoxComponent.h"

ARPGMapBoundsVolume::ARPGMapBoundsVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Bounds = CreateDefaultSubobject<UBoxComponent>(FName("Bounds"));
	RootComponent = Bounds;
	Bounds->SetBoxExtent(FVector(50000.f, 50000.f, 10000.f)); // Should change in editor
	Bounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARPGMapBoundsVolume::GetXYMinMax(FVector2D& OutMin, FVector2D& OutMax) const
{
	const FVector Location = Bounds->GetComponentLocation();
	const FVector Extent = Bounds->GetScaledBoxExtent();

	OutMin = FVector2D(Location.X - Extent.X, Location.Y - Extent.Y);
	OutMax = FVector2D(Location.X + Extent.X, Location.Y + Extent.Y);
}

