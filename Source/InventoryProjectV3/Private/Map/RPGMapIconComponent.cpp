// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapIconComponent.h"

#include "Map/RPGMapSubsystem.h"

URPGMapIconComponent::URPGMapIconComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Icon = nullptr;
	IconSize = FVector2D(24.f, 24.f);
	Tint = FLinearColor::White;
	bRotateWithActor = false;
	Category = EMapIconCategory::Custom;
	// TODO: THIS CLASS IS UNUSED. DELETE LATER
}
