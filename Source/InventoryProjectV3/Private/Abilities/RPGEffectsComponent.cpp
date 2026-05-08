// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGEffectsComponent.h"

URPGEffectsComponent::URPGEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URPGEffectsComponent::BeginPlay()
{
	Super::BeginPlay();
}


void URPGEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

