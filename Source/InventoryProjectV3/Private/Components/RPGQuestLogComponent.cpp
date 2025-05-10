// Oleksandr Tkachov 2022-2025


#include "Components/RPGQuestLogComponent.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGInventory_Component.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Items/RPGItem_Base.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "Quests/RPGQuest.h"
#include "Utility/LogDefinitions.h"
#include "Widgets/Quests/RPGQuestLogEntryWidget.h"
#include "Widgets/Quests/RPGQuestLogWidget.h"

URPGQuestLogComponent::URPGQuestLogComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ActiveQuests.Empty();
	CompleteQuests.Empty();
	CurrentActiveQuest = nullptr;
	QuestLogWidgetClass = nullptr;
	QuestLogWidget = nullptr;
	PlayerRef = nullptr;
}

void URPGQuestLogComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPGQuestLogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGQuestLogComponent::SetActiveQuest(ARPGQuest* Quest, bool bPlaySound /* = false */)
{
	CurrentActiveQuest = Quest;

	// TODO: WBP_Quest_HUD_Objectives::Update Objectives List - to be implemented

	if (QuestLogWidget)
	{
		QuestLogWidget->OnActiveQuestChanged();
	}

	CheckPlayerInventory(Quest);
	MarkQuestComplete(Quest->GetClass());

	if (bPlaySound)
	{
		// TODO: Post Ak audio event here
	}
	
	UE_LOG(LogQuests, Log, TEXT("[URPGQuestLogComponent::SetActiveQuest] Quest (%s) is now active"), *Quest->GetQuestName().ToString());
}

void URPGQuestLogComponent::AddQuest(ARPGQuest* Quest)
{
	if (!Quest)
	{
		UE_LOG(LogQuests, Error, TEXT("[URPGQuestLogComponent::AddQuest] Quest is nullptr"));
		return;
	}

	if (!GetActiveQuests().Contains(Quest))
	{
		GetActiveQuests().AddUnique(Quest);
		UE_LOG(LogQuests, Log, TEXT("[URPGQuestLogComponent::AddQuest] Quest (%s) added to quest log"), *Quest->GetQuestName().ToString());
	}
}

bool URPGQuestLogComponent::IsQuestAccepted(TSubclassOf<ARPGQuest> QuestClass) const
{
	return GetActiveQuests().Contains(GetSpawnedQuest(QuestClass));
}

bool URPGQuestLogComponent::IsQuestComplete(TSubclassOf<ARPGQuest> QuestClass) const
{
	// TODO: How do we update CompleteQuests array? (Currently in DeleteQuest)
	return GetCompleteQuests().Contains(GetSpawnedQuest(QuestClass));
}

void URPGQuestLogComponent::CheckPlayerInventory(ARPGQuest* Quest)
{
	if (!Quest)
	{
		return;
	}

	ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(GetOuter()); // TODO: Check this getter
	if (!PlayerCharacter)
	{
		return;
	}
	
	TArray<FObjectiveData> ObjectiveElement;
	TMap<int32, bool> IndicesOfSuccessfullyQuerriedObjectives;
	
	for (int i = 0; i < Quest->GetObjectives().Num(); i++)
	{
		const FObjectiveData Objective = Quest->GetObjectives()[i];
		if (Objective.Type == OT_Collect)
		{
			const ARPGItem_Base* ObjectiveItem = Cast<ARPGItem_Base>(Objective.Target.Get());
			if (ObjectiveItem)
			{
				ObjectiveElement.Add(Objective);

				int32 TempA, TempB;				
				if (bool bSuccessfullyQuerried = PlayerCharacter->GetInventoryComponent()->QueryInventory(ObjectiveItem->GetClass(), Objective.Amount, TempA, TempB))
				{
					IndicesOfSuccessfullyQuerriedObjectives.Add(i, bSuccessfullyQuerried);
				}
			}
		}
	}

	// There is at least one successfully querried objective
	// TODO: Recheck this since I don't know what's going on (this code was written 4 years ago)
	if (IndicesOfSuccessfullyQuerriedObjectives.Num() > 0)
	{
		TArray<int32> Keys;
		IndicesOfSuccessfullyQuerriedObjectives.GetKeys(Keys);

		TArray<bool> Values;
		IndicesOfSuccessfullyQuerriedObjectives.GenerateValueArray(Values);

		for (int i = 0; i < Keys.Num(); i++)
		{
			Quest->GetObjectives()[i].bCompleted = Values[i];
			Quest->GetObjectives()[i].bCanBeCompleted = Values[i];
		}
	}
}

void URPGQuestLogComponent::ToggleQuestLog()
{
	ARPGPlayer_Controller* RPGPlayerController = Cast<ARPGPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ensure(RPGPlayerController);
	check(QuestLogWidgetClass);
	
	if (QuestLogWidget)
	{
		QuestLogWidget->RemoveFromParent();
		QuestLogWidget = nullptr;

		UE_LOG(LogQuests, Log, TEXT("[URPGQuestLogComponent::ToggleQuestLog] QuestLogWidget removed"));
		
		RPGPlayerController->SetDefaultInputMode();
		return;
	}

	QuestLogWidget = Cast<URPGQuestLogWidget>(CreateWidget(GetWorld(), QuestLogWidgetClass));
	if (!QuestLogWidget)
	{
		UE_LOG(LogQuests, Error, TEXT("[URPGQuestLogComponent::ToggleQuestLog] QuestLogWidget was not created, probably because QuestLogWidgetClass is nullptr"));
	}

	QuestLogWidget->AddToViewport();
	QuestLogWidget->SetFocus();

	RPGPlayerController->SetUIInputMode();
}

bool URPGQuestLogComponent::MarkQuestComplete(TSubclassOf<ARPGQuest> QuestClass) const
{
	const ARPGQuest* Quest = GetSpawnedQuest(QuestClass);
	if (!Quest)
	{
		UE_LOG(LogQuests, Error, TEXT("[URPGQuestLogComponent::MarkQuestComplete] Provided quest doesn't exist"));
		return false;
	}
	
	for (const FObjectiveData Objective : Quest->GetObjectives())
	{
		if (!Objective.bCompleted)
		{
			// If at least one objective is incomplete we can't mark the quest complete
			UE_LOG(LogQuests, Warning, TEXT("[URPGQuestLogComponent::MarkQuestComplete] Quest (%s) is not complete since Objective (%s) is not complete"), *Quest->GetName(), *GetNameSafe(Objective.Target.Get()));
			return false;
		}
	}

	UE_LOG(LogQuests, Log, TEXT("[URPGQuestLogComponent::MarkQuestComplete] Quest (%s) is now marked complete"), *Quest->GetName());
	// TODO: Perhaps this is the best place to update CompleteQuests array? (for now it happens in DeleteQuest())
	return true;
}

void URPGQuestLogComponent::DeleteQuest(ARPGQuest* Quest, bool bAbandonQuest /* = false */)
{
	if (!Quest)
	{
		UE_LOG(LogQuests, Error, TEXT("[URPGQuestLogComponent::DeleteQuest] Provided quest doesn't exist"));
		return;
	}

	if (!QuestLogWidget)
	{
		UE_LOG(LogQuests, Error, TEXT("[URPGQuestLogComponent::DeleteQuest] QuestLogWidget is null"));
		return;
	}
	
	if (Quest->bCompleted)
	{
		CompleteQuests.AddUnique(Quest);
	}

	if (bAbandonQuest)
	{
		Quest->bCompleted = false;

		for (FObjectiveData Objective : Quest->GetObjectives())
		{
			Objective.bCanBeCompleted = false;
			Objective.bCompleted = false;
		}

		QuestLogWidget->SideQuestList->RemoveChild(Quest->QuestLogEntryRef);
		QuestLogWidget->StoryQuestList->RemoveChild(Quest->QuestLogEntryRef);

		Quest->QuestLogEntryRef = nullptr;

		UE_LOG(LogQuests, Warning, TEXT("[URPGQuestLogComponent::DeleteQuest] Quest %s was abandoned, resetting its objectives"), *Quest->GetQuestName().ToString());
		
		// TODO: Post quest fail sound
	}

	GetActiveQuests().Remove(Quest);

	SetActiveQuest(nullptr);

	if (GetActiveQuests().IsEmpty())
	{
		QuestLogWidget->QuestInfoBox->ClearChildren();
		QuestLogWidget->ActiveQuest = nullptr;
		// TODO: Check abandon button visibility here
		UE_LOG(LogQuests, Log, TEXT("[URPGQuestLogComponent::DeleteQuest] No quests in quest log"));
	}
	else
	{
		// Set first available quest to active
		// TODO: Probably can be replaced with ForEach without index
		for (int i = 0; i < GetActiveQuests().Num(); i++)
		{
			if (GetActiveQuests().IsValidIndex(i))
			{
				SetActiveQuest(GetActiveQuests()[i]);
				UE_LOG(LogQuests, Log, TEXT("[URPGQuestLogComponent::DeleteQuest] Set new active quest: %s"), *GetActiveQuests()[i]->GetQuestName().ToString());
				break;
			}
		}
	}
}

ARPGQuest* URPGQuestLogComponent::GetSpawnedQuest(TSubclassOf<ARPGQuest> QuestClass) const
{
	// TODO: Replace it with some kind of a quest manager or something - make new quest registered somewhere so they can be accessed anytime anywhere and it only supports one quest actor per actor class
	return Cast<ARPGQuest>(UGameplayStatics::GetActorOfClass(GetWorld(), QuestClass));
}
