// Oleksandr Tkachov 2022-2025


#include "Components/RPGStatsComponent.h"

URPGStatsComponent::URPGStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Strength = 10;
	Agility = 10;
	Intellect = 10;
	Stamina = 10;
	CritChance = 10;
	Speed = 10;
	
}

void URPGStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPGStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

