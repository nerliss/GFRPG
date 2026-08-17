// Oleksandr Tkachov 2021-2026


#include "Quests/RPGQuest.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGInventoryComponent.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/RPGXPComponent.h"
#include "Items/RPGItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Quests/RPGQuestMarkerLocation.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"
#include "Widgets/Quests/RPGQuestLogWidget.h"

ARPGQuest::ARPGQuest()
{
	PrimaryActorTick.bCanEverTick = false;

	QuestMarkComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuestMarkComponent"));
	QuestMarkComponent->SetupAttachment(RootComponent);
	QuestMarkComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
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
			LOG_WITH_FUNCTION_NAME(LogRPGQuests, Verbose, TEXT("Found required Location Objective, %s is now marked as completed"), *LocationObjective->GetName());
			MakeNearestObjectiveAvailable(i);
			Objectives[i].bCompleted = true;
			bUpdateUI = true;
			break; 
		}
	}
	
	UpdateUI(bUpdateUI);
}

void ARPGQuest::CheckItemObjective(ARPGItemBase* ItemTarget)
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
				if (PlayerCharacter->GetInventoryComponent()->QueryInventory(ItemTarget->GetClass(), Objective.Amount))
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
		LOG_WITH_FUNCTION_NAME(LogRPGQuests, Error, TEXT("Already at the last objective"));
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

		LOG_WITH_FUNCTION_NAME(LogRPGQuests, Verbose, TEXT("Objective %s is now active"), *GetNameSafe(Objectives[NearestIncompleteObjectiveIndex].Target.Get()));

		const ARPGItemBase* ObjectiveItem = Cast<ARPGItemBase>(Objectives[NearestIncompleteObjectiveIndex].Target.Get());
		if (ObjectiveItem)
		{
			const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (PlayerCharacter)
			{
				if (PlayerCharacter->GetInventoryComponent()->QueryInventory(ObjectiveItem->GetClass(), Objectives[NearestIncompleteObjectiveIndex].Amount))
				{
					// TODO: Test this when possible
					Objectives[NearestIncompleteObjectiveIndex].bCompleted = true;
					Objectives[NearestIncompleteObjectiveIndex].bCanBeCompleted = true;

					LOG_WITH_FUNCTION_NAME(LogRPGQuests, Verbose, TEXT("Objective's target is item (%s) and player has enough these items (%d) to automatically complete this objective"), *GetNameSafe(ObjectiveItem), Objectives[NearestIncompleteObjectiveIndex].Amount)

					MakeNearestObjectiveAvailable(ObjectiveIndex); // Probably needs to be ObjectiveIndex + 1 or something
				}
			}
		}
	}
	else
	{
		LOG_WITH_FUNCTION_NAME(LogRPGQuests, Warning, TEXT("No more objectives available"));
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

			LOG_WITH_FUNCTION_NAME(LogRPGQuests, Verbose, TEXT("Found active objective %s at %d index"), *GetNameSafe(Objective.Target.Get()), ObjectiveIndex);

			return true;
		}
	}

	LOG_WITH_FUNCTION_NAME(LogRPGQuests, Error, TEXT("No active objectives found"));
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

			LOG_WITH_FUNCTION_NAME(LogRPGQuests, Verbose, TEXT("Found next objective %s at %d index"), *GetNameSafe(Objective.Target.Get()), ObjectiveIndex);

			return true;
		}
	}

	ObjectiveIndex = 0;
	Objective = FObjectiveData();

	LOG_WITH_FUNCTION_NAME(LogRPGQuests, Error, TEXT("No next objective found"));

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
	LOG_WITH_FUNCTION_NAME(LogRPGQuests, Verbose, TEXT("Quest %s (object %s) now has updated XP Reward: %f"), *Name.ToString(), *GetName(), XPReward);
}

void ARPGQuest::OnQuestCompleted()
{
	bCompleted = true;

	// TODO: Decide if quest mark is present and update its visibility here
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
