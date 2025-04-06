// Oleksandr Tkachov 2022-2025


#include "Quests/RPGQuest.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGQuestLogComponent.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Quests/RPGQuestMarkerLocation.h"

ARPGQuest::ARPGQuest()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARPGQuest::BeginPlay()
{
	Super::BeginPlay();

	OnTargetInteracted.AddDynamic(this, &ARPGQuest::CheckInteractionObjective);
	OnLocationReached.AddDynamic(this, &ARPGQuest::CheckLocationObjective);
	OnItemCollected.AddDynamic(this, &ARPGQuest::CheckItemObjective);
	OnEnemyKilled.AddDynamic(this, &ARPGQuest::CheckKillObjective);
}

void ARPGQuest::CheckInteractionObjective(AActor* InteractionTarget)
{

}

void ARPGQuest::CheckLocationObjective(ARPGQuestMarkerLocation* LocationTarget)
{
	//int32 ObjectiveIndex = 0;
	bool bUpdateUI = false;

	for (int i = 0; i < Objectives.Num(); i++)
	{
		auto Objective = Objectives[i];
		int32 ObjectiveIndex = i;

		auto ObjectiveTargetCasted = Cast<ARPGQuestMarkerLocation>(Objective.Target.Get());
		if (!Objective.bCompleted && Objective.bCanBeCompleted && ObjectiveTargetCasted == LocationTarget)
		{
			MakeNearestObjectiveAvailable(ObjectiveIndex);

			FObjectiveData UpdatedObjective = Objectives[ObjectiveIndex];
			UpdatedObjective.bCompleted = true;

			Objectives[ObjectiveIndex] = UpdatedObjective;

			bUpdateUI = true;
		}
	}

	if (bUpdateUI)
	{
		// TODO: Create a static library with getters for project-specific classes (like a quick access to RPGPlayerCharacter)
		auto PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerCharacter)
		{
			PlayerCharacter->GetQuestLogComponent()->SetActiveQuest(this, true);
		}
	}
}

void ARPGQuest::CheckItemObjective(ARPGItem_Base* ItemTarget)
{

}

void ARPGQuest::CheckKillObjective(ARPGCharacter* KillTarget)
{

}

void ARPGQuest::MakeNearestObjectiveAvailable(int32 ObjectiveIndex)
{

}
