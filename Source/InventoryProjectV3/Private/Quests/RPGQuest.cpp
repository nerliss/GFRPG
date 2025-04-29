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
#include "Widgets/Quests/RPGQuestLogWidget.h"

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
	QuestLogEntryRef = nullptr;
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
	bool bUpdateUI = false;
	
	for (int i = 0; i < Objectives.Num(); i++)
	{
		FObjectiveData Objective = Objectives[i];

		if (!Objective.bCompleted && Objective.bCanBeCompleted && InteractionTarget == Objective.Target.Get())
		{
			MakeNearestObjectiveAvailable(i);
			Objectives[i].bCompleted = true;
			bUpdateUI = true;
			break;
		}
	}
	
	UpdateUI(bUpdateUI);
}

void ARPGQuest::CheckLocationObjective(ARPGQuestMarkerLocation* LocationTarget)
{
	bool bUpdateUI = false;

	for (int i = 0; i < Objectives.Num(); i++)
	{
		FObjectiveData Objective = Objectives[i];

		const ARPGQuestMarkerLocation* LocationObjective = Cast<ARPGQuestMarkerLocation>(Objective.Target.Get());
		if (!Objective.bCompleted && Objective.bCanBeCompleted && LocationObjective == LocationTarget)
		{
			UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::CheckLocationObjective] Found required Location Objective, %s is now marked as completed"), *LocationObjective->GetName());
			MakeNearestObjectiveAvailable(i);
			Objectives[i].bCompleted = true;
			bUpdateUI = true;
			break; 
		}
	}
	
	UpdateUI(bUpdateUI);
}

void ARPGQuest::CheckItemObjective(ARPGItem_Base* ItemTarget)
{
	bool bUpdateUI = false;

	for (int i = 0; i < Objectives.Num(); i++)
	{
		FObjectiveData Objective = Objectives[i];

		if (!Objective.bCompleted && Objective.bCanBeCompleted && ItemTarget->GetClass() == Objective.Target->GetClass())
		{
			// TODO: WBP_QuestHUDObjectives::UpdateObjectivesList() - not yet implemented

			const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			if (PlayerCharacter)
			{
				int32 LocalA, LocalB;
				if (PlayerCharacter->GetInventoryComponent()->QueryInventory(ItemTarget->GetClass(), Objective.Amount, LocalA, LocalB))
				{
					MakeNearestObjectiveAvailable(i);
					Objectives[i].bCompleted = true;
					bUpdateUI = true;
					break;
				}
			}
		}
	}

	UpdateUI(bUpdateUI);
}

void ARPGQuest::CheckKillObjective(ARPGCharacter* KillTarget)
{
	bool bUpdateUI = false;

	for (int i = 0; i < Objectives.Num(); i++)
	{
		FObjectiveData Objective = Objectives[i];
		
		if (!Objective.bCompleted && Objective.bCanBeCompleted && KillTarget->GetClass() == Objective.Target->GetClass())
		{
			KillCountCurrent++;

			// TODO: WBP_QuestHUDObjectives::UpdateObjectivesList() - not yet implemented

			if (KillCountCurrent >= Objective.Amount)
			{
				MakeNearestObjectiveAvailable(i);
				Objectives[i].bCompleted = true;
				bUpdateUI = true;
				break;
			}
		}
	}

	UpdateUI(bUpdateUI);
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
		Objectives[NearestIncompleteObjectiveIndex].bCanBeCompleted = true;

		UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::MakeNearestObjectiveAvailable] Objective %s is now active"), *GetNameSafe(Objectives[NearestIncompleteObjectiveIndex].Target.Get()));

		const ARPGItem_Base* ObjectiveItem = Cast<ARPGItem_Base>(Objectives[NearestIncompleteObjectiveIndex].Target.Get());
		if (ObjectiveItem)
		{
			const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (PlayerCharacter)
			{
				int32 LocalA, LocalB;
				if (PlayerCharacter->GetInventoryComponent()->QueryInventory(ObjectiveItem->GetClass(), Objectives[NearestIncompleteObjectiveIndex].Amount, LocalA, LocalB))
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
	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		return;
	}

	XPReward = FMath::CeilToFloat(PlayerCharacter->XPComp->CalculateXPReward(true, XPRewardMultiplier));
	UE_LOG(LogQuests, Verbose, TEXT("[ARPGQuest::CalculateXP] Quest %s (object %s) now has updated XP Reward: %f"), *Name.ToString(), *GetName(), XPReward);
}

void ARPGQuest::UpdateUI(bool bNeedsUIUpdate)
{
	if (!bNeedsUIUpdate)
	{
		return;
	}
	
	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter)
	{
		PlayerCharacter->GetQuestLogComponent()->SetActiveQuest(this, true);
	}
}
