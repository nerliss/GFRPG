// Oleksandr Tkachov 2022-2025


#include "Quests/RPGQuest.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGInventory_Component.h"
#include "Components/RPGQuestLogComponent.h"
#include "Items/RPGItem_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Quests/RPGQuestMarkerLocation.h"
#include "Utility/LogDefinitions.h"

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
		FObjectiveData Objective = Objectives[i];
		int32 ObjectiveIndex = i;

		auto ObjectiveTargetCasted = Cast<ARPGQuestMarkerLocation>(Objective.Target.Get());
		if (!Objective.bCompleted && Objective.bCanBeCompleted && ObjectiveTargetCasted == LocationTarget)
		{
			UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::CheckLocationObjective] Found required Location Objective, %s is now marked as completed"), *ObjectiveTargetCasted->GetName());
			MakeNearestObjectiveAvailable(ObjectiveIndex);

			// TODO: It seems it can be replace with simple array index access, without creating a copy and then replacing the index
			FObjectiveData UpdatedObjective = Objectives[ObjectiveIndex];
			UpdatedObjective.bCompleted = true;
			Objectives[ObjectiveIndex] = UpdatedObjective;

			bUpdateUI = true;
			break; // Not sure if required here
		}
	}

	if (bUpdateUI)
	{
		// TODO: Create a static library with getters for project-specific classes (like a quick access to RPGPlayerCharacter)
		auto PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerCharacter)
		{
			UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::CheckLocationObjective] Updating UI for Quest %s"), *GetName());
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
	if (ObjectiveIndex == Objectives.Num() - 1)
	{
		UE_LOG(LogQuests, Error, TEXT("[ARPGQuest::MakeNearestObjectiveAvailable] Already at the last objective"));
		return;
	}

	bool bObjectiveFound = false;
	int32 NearestIncompleteObjectiveIndex = -1;

	for (int i = 0; i < Objectives.Num(); i++)
	{
		FObjectiveData Objective = Objectives[i];
		if (ObjectiveIndex != i && !Objective.bCompleted)
		{
			NearestIncompleteObjectiveIndex = i;
			bObjectiveFound = true;
			break;
		}
	}

	if (bObjectiveFound)
	{
		// TODO: It seems it can be replace with simple array index access, without creating a copy and then replacing the index
		FObjectiveData UpdatedObjective = Objectives[NearestIncompleteObjectiveIndex];
		UpdatedObjective.bCanBeCompleted = true;
		Objectives[NearestIncompleteObjectiveIndex] = UpdatedObjective;

		UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::MakeNearestObjectiveAvailable] Objective %s is now active"), *GetNameSafe(UpdatedObjective.Target.Get()));

		auto ObjectiveItem = Cast<ARPGItem_Base>(Objectives[NearestIncompleteObjectiveIndex].Target.Get());
		if (ObjectiveItem)
		{
			auto PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (PlayerCharacter)
			{
				int32 LocalA, LocalB;
				if (PlayerCharacter->InventoryComp->QueryInventory(ObjectiveItem->GetClass(), Objectives[NearestIncompleteObjectiveIndex].Amount, LocalA, LocalB))
				{
					// TODO: Set this new objective to completed, and call MakeNearestObjectiveAvailable againg
				}
			}
		}
	}
	else
	{
		UE_LOG(LogQuests, Warning, TEXT("[ARPGQuest::MakeNearestObjectiveAvailable] No more objectives available"));
	}
}

bool ARPGQuest::GetActiveObjective(int32& ObjectiveIndex, FObjectiveData& Objective) const
{
	// TODO: Continue here
	return false;
}
