// Oleksandr Tkachov 2022-2025


#include "Quests/RPGQuest.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGInventory_Component.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/RPGXP_Component.h"
#include "Items/RPGItem_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Quests/RPGQuestMarkerLocation.h"
#include "Utility/LogDefinitions.h"

ARPGQuest::ARPGQuest()
{
	PrimaryActorTick.bCanEverTick = false;

	Name = FText::FromString("Quest Name");
	Description = FText::FromString("Quest Description");
	Objectives.Empty();
	bStoryQuest = false;
	bCompleted = false;
	KillCountCurrent = -1;
	XPReward = 10.f;
	XPRewardMultiplier = 1.f;
	MoneyReward = 1.f;
	TurnInText = FText::FromString("Turn In Text");
	Prerequisite = nullptr;
	QuestGiver = nullptr;
	bCanBeTurnedInToSomeoneElse = false;
	QuestTurnInTarget = nullptr;
	PlayerRef = nullptr;
	PCRef = nullptr;
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
		const int32 ObjectiveIndex = i;

		const auto LocationObjective = Cast<ARPGQuestMarkerLocation>(Objective.Target.Get());
		if (!Objective.bCompleted && Objective.bCanBeCompleted && LocationObjective == LocationTarget)
		{
			UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::CheckLocationObjective] Found required Location Objective, %s is now marked as completed"), *LocationObjective->GetName());
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
		const auto PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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

		const auto ObjectiveItem = Cast<ARPGItem_Base>(Objectives[NearestIncompleteObjectiveIndex].Target.Get());
		if (ObjectiveItem)
		{
			const auto PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (PlayerCharacter)
			{
				int32 LocalA, LocalB;
				if (PlayerCharacter->InventoryComp->QueryInventory(ObjectiveItem->GetClass(), Objectives[NearestIncompleteObjectiveIndex].Amount, LocalA, LocalB))
				{
					// TODO: Test this when possible
					Objectives[NearestIncompleteObjectiveIndex].bCompleted = true;
					Objectives[NearestIncompleteObjectiveIndex].bCanBeCompleted = true;

					UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::MakeNearestObjectiveAvailable] Objective's target is item (%s) and player has enough these items (%d) to automatically complete this objective"), *GetNameSafe(ObjectiveItem), Objectives[NearestIncompleteObjectiveIndex].Amount)

					MakeNearestObjectiveAvailable(ObjectiveIndex); // Probably needs to be ObjectiveIndex + 1 or something
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
	for (int i = 0; i < Objectives.Num(); i++)
	{
		FObjectiveData LocalObjective = Objectives[i];
		if (!LocalObjective.bCompleted && LocalObjective.bCanBeCompleted)
		{
			ObjectiveIndex = i;
			Objective = LocalObjective;

			UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::GetActiveObjective] Found active objective %s at %d index"), *GetNameSafe(Objective.Target.Get()), ObjectiveIndex);

			return true;
		}
	}

	UE_LOG(LogQuests, Error, TEXT("[ARPGQuest::GetActiveObjective] No active objectives found"));
	return false;
}

bool ARPGQuest::GetNextObjective(int32& ObjectiveIndex, FObjectiveData& Objective) const
{
	int LocalObjectiveIndex;
	FObjectiveData LocalObjective;

	if (GetActiveObjective(LocalObjectiveIndex, LocalObjective))
	{
		const int32 NextObjectiveIndex = LocalObjectiveIndex + 1;

		if (Objectives.IsValidIndex(NextObjectiveIndex))
		{
			ObjectiveIndex = NextObjectiveIndex;
			Objective = Objectives[NextObjectiveIndex];

			UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::GetNextObjective] Found next objective %s at %d index"), *GetNameSafe(Objective.Target.Get()), ObjectiveIndex);

			return true;
		}
	}

	ObjectiveIndex = 0;
	Objective = FObjectiveData();

	UE_LOG(LogQuests, Error, TEXT("[ARPGQuest::GetNextObjective] No next objective found"));

	return false;
}

void ARPGQuest::CalculateXP()
{
	const auto PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		return;
	}

	XPReward = FMath::CeilToFloat(PlayerCharacter->XPComp->CalculateXPReward(true, XPRewardMultiplier));
	UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::CalculateXP] Quest %s (object %s) now has updated XP Reward: %f"), *Name.ToString(), *GetName(), XPReward);
}
