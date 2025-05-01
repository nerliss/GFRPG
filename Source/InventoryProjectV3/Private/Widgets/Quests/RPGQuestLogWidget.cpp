// Oleksandr Tkachov 2022-2025


#include "Widgets/Quests/RPGQuestLogWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "GameFramework/InputSettings.h"
#include "Quests/RPGQuest.h"
#include "Widgets/Quests/RPGQuestLogEntryWidget.h"
#include "Widgets/Quests/RPGQuestObjectiveItemWidget.h"

void URPGQuestLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshQuestLogUI();

	// TODO: Check Abandon Quest Button visibility (if abandon feature is present at all)
}

FReply URPGQuestLogWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<FInputActionKeyMapping> ToggleQuestLogMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("ToggleQuestLog"), ToggleQuestLogMappings);

	for (FInputActionKeyMapping Mapping: ToggleQuestLogMappings)
	{
		if (InKeyEvent.GetKey() == Mapping.Key)
		{
			PlayerQuestLog->ToggleQuestLog();
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void URPGQuestLogWidget::OnActiveQuestChanged()
{
	ObjectiveList->ClearChildren();

	const ARPGPlayerCharacter* RPGCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!RPGCharacter)
	{
		return;
	}

	ActiveQuest = RPGCharacter->GetQuestLogComponent()->GetCurrentActiveQuest();
	if (!ActiveQuest)
	{
		return;
	}
	
	for (FObjectiveData Objective : ActiveQuest->GetObjectives())
	{
		// TODO: Retest the condition since I don't really understand why Collect type can be shown while completed 
		if (Objective.bCanBeCompleted || (Objective.Type == OT_Collect && Objective.bCompleted))
		{
			URPGQuestObjectiveItemWidget* ObjectiveItemWidget = Cast<URPGQuestObjectiveItemWidget>(CreateWidget(GetWorld(), ObjectiveItemClass));
			if (ObjectiveItemWidget)
			{
				// TODO: Remove this since we probably don't need this
				// ObjectiveItemWidget->ObjectiveText = Objective.Description;
				// ObjectiveItemWidget->bCompleted = Objective.bCompleted;
				ObjectiveItemWidget->Objective = Objective;

				ObjectiveList->AddChild(ObjectiveItemWidget);
			}

			// TODO: Should it really be here and not after the loop?
			RewardBox->SetVisibility(ActiveQuest ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
}

void URPGQuestLogWidget::RefreshQuestLogUI()
{
	const ARPGPlayerCharacter* RPGCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (RPGCharacter)
	{
		PlayerQuestLog = RPGCharacter->GetQuestLogComponent();
	}
	
	for (ARPGQuest* Quest : PlayerQuestLog->GetActiveQuests())
	{
		if (Quest)
		{
			URPGQuestLogEntryWidget* QuestLogEntryWidget = CreateWidget<URPGQuestLogEntryWidget>(GetWorld(), QuestLogEntryWidgetClass);
			if (QuestLogEntryWidget)
			{
				QuestLogEntryWidget->Quest = Quest;
				Quest->QuestLogEntryRef = QuestLogEntryWidget;

				if (Quest->bStoryQuest)
				{
					StoryQuestList->AddChild(QuestLogEntryWidget);
				}
				else
				{
					SideQuestList->AddChild(QuestLogEntryWidget);
				}
			}
		}
	}
	
	OnActiveQuestChanged();
	PlayConstructionFX();
}

void URPGQuestLogWidget::PlayConstructionFX()
{
	// TODO: Post Ak audio event here
}
