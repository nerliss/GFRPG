// Oleksandr Tkachov 2022-2025


#include "Quests/RPGQuestMarkerLocation.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/SphereComponent.h"
#include "Quests/RPGQuest.h"
#include "Utility/LogDefinitions.h"

ARPGQuestMarkerLocation::ARPGQuestMarkerLocation()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	//SetRootComponent(SphereCollisionComponent);
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
	const auto* PlayerCharacter = Cast<ARPGPlayerCharacter>(OtherActor);
	if (!PlayerCharacter)
	{
		return;
	}

	if (PlayerCharacter->GetQuestLogComponent())
	{
		for (const ARPGQuest* Quest : PlayerCharacter->GetQuestLogComponent()->GetActiveQuests())
		{
			if (Quest)
			{
				for (const FObjectiveData Objective : Quest->GetObjectives())
				{
					if (Objective.Type == OT_Location && !Objective.bCompleted && Objective.bCanBeCompleted)
					{
						Quest->OnLocationReached.Broadcast(this);
						UE_LOG(LogRPGQuests, Warning, TEXT("Reached Location Objective %s (object name: %s)"), *Name.ToString(), *GetName());
						break;
					}
				}
			}
		}
	}
}
