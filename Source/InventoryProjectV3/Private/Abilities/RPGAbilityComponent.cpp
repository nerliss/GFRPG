// Oleksandr Tkachov 2022-2025


#include "Abilities/RPGAbilityComponent.h"

URPGAbilityComponent::URPGAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URPGAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPGAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

