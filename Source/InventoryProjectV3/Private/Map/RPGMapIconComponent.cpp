// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapIconComponent.h"

#include "Map/RPGMapSubsystem.h"

URPGMapIconComponent::URPGMapIconComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void URPGMapIconComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
		if (MapSubsystem.IsValid())
		{
			MapSubsystem->RegisterIcon(this);
		}
	}
}

void URPGMapIconComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (MapSubsystem.IsValid())
	{
		MapSubsystem->UnregisterIcon(this);
	}
	
	Super::EndPlay(EndPlayReason);
}

bool URPGMapIconComponent::GetMapUV(FVector2D& OutUV) const
{
	if (!MapSubsystem.IsValid() || !GetOwner())
	{
		return false;
	}

	return MapSubsystem->WorldToMapUV(GetOwner()->GetActorLocation(), OutUV);
}

