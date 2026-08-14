// Oleksandr Tkachov 2021-2026


#include "Quests/RPGQuestMarkerLocation.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/SphereComponent.h"
#include "Quests/RPGQuest.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

ARPGQuestMarkerLocation::ARPGQuestMarkerLocation()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	SphereCollisionComponent->SetupAttachment(RootComponent);
	SphereCollisionComponent->SetSphereRadius(320.f);

	PointOfInterestComponent = CreateDefaultSubobject<URPGPointOfInterestComponent>(TEXT("PointOfInterestComponent"));
	PointOfInterestComponent->bQuestObjective = true;

	Name = FText();
}

void ARPGQuestMarkerLocation::BeginPlay()
{
	Super::BeginPlay();

	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPGQuestMarkerLocation::OnSphereBeginOverlap);

}

void ARPGQuestMarkerLocation::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(OtherActor);
	if (!PlayerCharacter)
	{
		return;
	}

	if (!PlayerCharacter->GetQuestLogComponent())
	{
		return;
	}

	for (const ARPGQuest* Quest : PlayerCharacter->GetQuestLogComponent()->GetActiveQuests())
	{
		if (!Quest)
		{
			continue;
		}

		// TODO: May be add some additional check for this specific marker location and compare against it
		for (const FObjectiveData Objective : Quest->GetObjectives())
		{
			if (Objective.Type == OT_Location && !Objective.bCompleted && Objective.bCanBeCompleted)
			{
				Quest->OnLocationReached.Broadcast(this);
				LOG_WITH_FUNCTION_NAME(LogRPGQuests, Warning, TEXT("Reached Location Objective %s (object name: %s)"), *Name.ToString(), *GetName());
				break;
			}
		}
	}
}
