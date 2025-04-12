// Oleksandr Tkachov 2022-2025


#include "Components/RPGQuestLogComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Quests/RPGQuest.h"
#include "Utility/LogDefinitions.h"

URPGQuestLogComponent::URPGQuestLogComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ActiveQuests.Empty();
	CompleteQuests.Empty();
	CurrentActiveQuest = nullptr;
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

void URPGQuestLogComponent::SetActiveQuest(ARPGQuest* Quest, bool bPlaySound)
{
	CurrentActiveQuest = Quest;

	// TODO: WBP_Quest_HUD_Objectives::Update Objectives List - to be implemented

	// TODO: WBP_Quest_Log::OnActiveQuestChanged - to be implemented

	CheckPlayerInventory(Quest);
	MarkQuestComplete(Quest->GetClass());

	if (bPlaySound)
	{
		// TODO: Post Ak audio event here
	}
}

void URPGQuestLogComponent::AddQuest(ARPGQuest* Quest)
{
	if (!Quest)
	{
		return;
	}

	if (!GetActiveQuests().Contains(Quest))
	{
		GetActiveQuests().AddUnique(Quest);
		UE_LOG(LogQuests, Log, TEXT("[URPGQuestLogComponent::AddQuest] Quest (%s) added to quest log"), *Quest->GetName());
	}
}

bool URPGQuestLogComponent::IsQuestAccepted(TSubclassOf<ARPGQuest> QuestClass) const
{
	return GetActiveQuests().Contains(GetSpawnedQuest(QuestClass));
}

bool URPGQuestLogComponent::IsQuestComplete(TSubclassOf<ARPGQuest> QuestClass) const
{
	// TODO: How do we update CompleteQuests array?
	return GetCompleteQuests().Contains(GetSpawnedQuest(QuestClass));
}

void URPGQuestLogComponent::CheckPlayerInventory(ARPGQuest* Quest)
{
	// TODO: Move from BP
}

void URPGQuestLogComponent::ToggleQuestLog()
{
	// TODO: Move from BP - Requires QuestLogRef creation
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
	// TODO: Perhaps this is the best place to update CompleteQuests array?
	return true;
}

ARPGQuest* URPGQuestLogComponent::GetSpawnedQuest(TSubclassOf<ARPGQuest> QuestClass) const
{
	// TODO: Replace it with some kind of a quest manager or something - make new quest registered somewhere so they can be accessed anytime anywhere and it only supports one quest actor per actor class
	return Cast<ARPGQuest>(UGameplayStatics::GetActorOfClass(GetWorld(), QuestClass));
}
