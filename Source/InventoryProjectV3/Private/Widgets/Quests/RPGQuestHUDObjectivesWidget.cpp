// Oleksandr Tkachov 2022-2025


#include "Widgets/Quests/RPGQuestHUDObjectivesWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Quests/RPGQuest.h"
#include "Widgets/Quests/RPGQuestHUDObjectiveEntryWidget.h"

void URPGQuestHUDObjectivesWidget::UpdateObjectiveList()
{
	ObjectivesListBox->ClearChildren();

	const ARPGPlayerCharacter* RPGCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!RPGCharacter)
	{
		return;
	}

	const URPGQuestLogComponent* QuestLog = RPGCharacter->GetQuestLogComponent();
	if (!QuestLog)
	{
		return;
	}
	
	const ARPGQuest* ActiveQuest = QuestLog->GetCurrentActiveQuest();
	if (!ActiveQuest)
	{
		return;
	}

	Objectives = ActiveQuest->GetObjectives();
	QuestName->SetText(ActiveQuest->GetQuestName());

	check(ObjectiveHUDEntryWidgetClass);

	if (QuestLog->MarkQuestComplete(ActiveQuest->GetClass()))
	{
		URPGQuestHUDObjectiveEntryWidget* ObjectiveHUDEntryWidget = Cast<URPGQuestHUDObjectiveEntryWidget>(
			CreateWidget(GetWorld(), ObjectiveHUDEntryWidgetClass));
		if (ObjectiveHUDEntryWidget)
		{
			// TODO: Recheck this and remove if possible - probably a blueprint limitation
			ObjectiveHUDEntryWidget->Objective = Objectives[0];
			ObjectiveHUDEntryWidget->bQuestComplete = true;

			// TODO: Rewrite this when QuestTurnInTarget = QuestGiver if bCanBeTurnedInToSomeoneElse == false is implemented
			FText NpcName;
			const ARPGCharacter* QuestGiverNPC = Cast<ARPGCharacter>(ActiveQuest->QuestGiver.Get());
			const ARPGCharacter* OtherTurnInNPC = Cast<ARPGCharacter>(ActiveQuest->QuestTurnInTarget.Get());
			if (OtherTurnInNPC || QuestGiverNPC)
			{
				NpcName = ActiveQuest->bCanBeTurnedInToSomeoneElse ? OtherTurnInNPC->GetCharacterName() : QuestGiverNPC->GetCharacterName();
			}
			
			const FString CompletionString = FString::Printf(
				TEXT("Quest Complete! \n %s to %s"),
				ActiveQuest->bCanBeTurnedInToSomeoneElse ? TEXT("Speak") : TEXT("Return"),
				*NpcName.ToString());
			ObjectiveHUDEntryWidget->CompletionText = FText::FromString(CompletionString);

			ObjectivesListBox->AddChild(ObjectiveHUDEntryWidget);
		}
	}
	else
	{
		for (FObjectiveData Objective: Objectives)
		{
			if (Objective.bCanBeCompleted || (Objective.Type == OT_Collect && Objective.bCompleted))
			{
				URPGQuestHUDObjectiveEntryWidget* ObjectiveHUDEntryWidget = Cast<URPGQuestHUDObjectiveEntryWidget>(CreateWidget(GetWorld(), ObjectiveHUDEntryWidgetClass));
				if (ObjectiveHUDEntryWidget)
				{
					ObjectiveHUDEntryWidget->Objective = Objective;
					ObjectiveHUDEntryWidget->UpdateTrackerData();
					
					ObjectivesListBox->AddChild(ObjectiveHUDEntryWidget);
				}
			}
		}
	}
}

void URPGQuestHUDObjectivesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateObjectiveList();
}
