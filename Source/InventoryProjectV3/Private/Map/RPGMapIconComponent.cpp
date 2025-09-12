// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapIconComponent.h"

URPGMapIconComponent::URPGMapIconComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URPGMapIconComponent::BeginPlay()
{
	Super::BeginPlay();

}

void URPGMapIconComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

