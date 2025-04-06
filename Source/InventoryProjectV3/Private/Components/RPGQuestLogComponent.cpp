// Oleksandr Tkachov 2022-2025


#include "Components/RPGQuestLogComponent.h"

URPGQuestLogComponent::URPGQuestLogComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URPGQuestLogComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPGQuestLogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGQuestLogComponent::SetActiveQuest(ARPGQuest* Quest, bool bPlaySound)
{
	// Some logic from BPs
}
