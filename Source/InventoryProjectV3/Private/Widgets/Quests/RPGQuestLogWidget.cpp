// Oleksandr Tkachov 2022-2025


#include "Widgets/Quests/RPGQuestLogWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/ScrollBox.h"
#include "GameFramework/InputSettings.h"
#include "Quests/RPGQuest.h"

void URPGQuestLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshQuestLogUI();
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
	if (RPGCharacter)
	{
		ActiveQuest = RPGCharacter->GetQuestLogComponent()->GetCurrentActiveQuest();
		if (ActiveQuest)
		{
			for (auto Objective : ActiveQuest->GetObjectives())
			{
				if (Objective.bCanBeCompleted || (Objective.Type == OT_Collect && Objective.bCompleted))
				{
					// TODO: Spawn QuestObjectiveItemWidget
				}
			}
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

	// TODO: QuestLogEntryWidget creation for each loop

	OnActiveQuestChanged();
	PlayConstructionFX();
}

void URPGQuestLogWidget::PlayConstructionFX()
{
	// TODO: Post Ak audio event here
}
