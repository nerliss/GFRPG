// Oleksandr Tkachov 2021-2026


#include "Widgets/Quests/RPGQuestHUDObjectiveEntryWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGInventory_Component.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/TextBlock.h"
#include "Items/RPGItem_Base.h"

void URPGQuestHUDObjectiveEntryWidget::UpdateTrackerData()
{
	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}
	
	switch (Objective.Type)
	{
	case OT_Location:
	case OT_Interact:
	case OT_Max:
	default:
		{
			ObjectiveTrackerTextBlock->SetText(FText());
			break;
		}
	case OT_Collect:
		{
			const ARPGItem_Base* ObjectiveItem = Cast<ARPGItem_Base>(Objective.Target.Get());
			if (ObjectiveItem)
			{
				int32 TempA, QueriedAmount;
				PlayerCharacter->GetInventoryComponent()->QueryInventory(ObjectiveItem->GetClass(), Objective.Amount,TempA,QueriedAmount);
				
				const int32 CurrentItemsAmount = FMath::Clamp(QueriedAmount, 0, Objective.Amount);
				const FText FormattedText = FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(CurrentItemsAmount), FText::AsNumber(Objective.Amount));
				ObjectiveTrackerTextBlock->SetText(FormattedText);
				break;
			}
		}
	case OT_Kill:
		{
			const int32 CurrentKillCount = PlayerCharacter->GetQuestLogComponent()->GetCurrentActiveQuest()->GetKillCountCurrent();
			const FText FormattedText = FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(CurrentKillCount), FText::AsNumber(Objective.Amount));
			ObjectiveTrackerTextBlock->SetText(FormattedText);
			break;
		}
	}
}

FText URPGQuestHUDObjectiveEntryWidget::GetTrackerData() const
{
	// TODO: Probably unused, remove later
	return FText();
}

FText URPGQuestHUDObjectiveEntryWidget::GetQuestStatusText() const
{
	if (Objective.bCompleted)
	{
		if (bQuestComplete)
		{
			ObjectiveDescriptionTextBlock->SetText(CompletionText);
			ObjectiveTrackerTextBlock->SetText(FText());
			return FText();
		}

		ObjectiveDescriptionTextBlock->SetColorAndOpacity(IncompleteColor);
		ObjectiveTrackerTextBlock->SetColorAndOpacity(IncompleteColor);
		ObjectiveStatusTextBlock->SetColorAndOpacity(IncompleteColor);
		return StatusText;
	}

	return FText::FromString("- ");
}
