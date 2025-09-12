// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapSubsystem.h"

void URPGMapSubsystem::SetBoundsVolume(ARPGMapBoundsVolume* Volume)
{
	
}

bool URPGMapSubsystem::WorldToMapUV(const FVector& World, FVector2D& OutUV) const
{
	return true;
}

FVector URPGMapSubsystem::MapUVToWorld(const FVector2D& UV, float Z) const
{
	return FVector(0, 0, 0);
}

void URPGMapSubsystem::RegisterIcon(URPGMapComponent* Icon)
{
	
}

void URPGMapSubsystem::UnregisterIcon(URPGMapComponent* Icon)
{
	
}

void URPGMapSubsystem::RevealAtWorld(const FVector& World, float RadiusWorldUnits, float Opacity)
{
	
}
