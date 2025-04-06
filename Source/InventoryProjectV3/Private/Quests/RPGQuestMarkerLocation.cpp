// Oleksandr Tkachov 2022-2025


#include "Quests/RPGQuestMarkerLocation.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Components/SphereComponent.h"

ARPGQuestMarkerLocation::ARPGQuestMarkerLocation()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	//SetRootComponent(SphereCollisionComponent);
	SphereCollisionComponent->SetupAttachment(RootComponent);
	SphereCollisionComponent->SetSphereRadius(320.f);

	PointOfInterestComponent = CreateDefaultSubobject<URPGPointOfInterestComponent>(TEXT("PointOfInterestComponent"));
	PointOfInterestComponent->bQuestObjective = true;
}

void ARPGQuestMarkerLocation::BeginPlay()
{
	Super::BeginPlay();
	
}
